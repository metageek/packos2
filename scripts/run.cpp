#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

bool fileExecutable(const char* path) {
  struct stat buf;
  if (stat(path, &buf)<0)
    return false;

  if (buf.st_uid == geteuid()) {
    return (buf.st_mode & S_IXUSR) != 0;
  } else {
    if (buf.st_gid == getegid()) {
      return (buf.st_mode & S_IXGRP) != 0;
    } else {
      return (buf.st_mode & S_IXOTH) != 0;
    }
  }
}

std::string parentOf(const char* path) {
  if (!strcmp(path, ".")) {
    return "..";
  }

  char* path_modifiable = strdup(path);

  char* slash=strrchr(path_modifiable, '/');
  if (!slash) {
    free(path_modifiable);
    return ".";
  }

  *slash=0;
  std::string res = path_modifiable;
  free(path_modifiable);
  return res;
}

std::string findScriptDir(const char* script) {
  if (fileExecutable(script)) {
    return parentOf(script);
  }

  const char* PATH=getenv("PATH");

  if (!PATH) { // Can't happen, I think: if no PATH, how did user find us?
    return "";
  }

  char* PATH_modifiable = strdup(PATH);
  for (char* p = PATH_modifiable; ; p = NULL) {
    const char* dir = strtok(p, ":");
    if (!dir) {
      break;
    }

    if (!dir[0]) {
      continue;
    }

    std::string f = dir;
    if (dir[strlen(dir)-1] != '/') {
      f += '/';
    }

    f += script;

    if (fileExecutable(f.c_str())) {
      free(PATH_modifiable);
      return parentOf(f.c_str());
    }
  }

  free(PATH_modifiable);
  // Can't happen, I think.
  return "";
}

int cp(const char* src, const char* dest) {
  int src_fd = open(src, O_RDONLY);
  if (src_fd < 0) {
    int tmp=errno;
    perror("open(src)");
    errno=tmp;
    return -1;
  }

  int dest_fd = open(dest, O_WRONLY | O_CREAT, 0600);
  if (dest_fd < 0) {
    int tmp=errno;
    perror("open(dest)");
    close(src_fd);
    errno=tmp;
    return -1;
  }

  while (true) {
    char buff[1024];
    int nbytes = read(src_fd, buff, sizeof(buff));
    if (nbytes == 0) { // eof
      break;
    }

    if (nbytes < 0) {
      int tmp=errno;
      perror("read");
      close(src_fd);
      close(dest_fd);
      errno=tmp;
      return -1;
    }

    int written = write(dest_fd, buff, nbytes);
    if (written < 0) {
      int tmp=errno;
      perror("write");
      close(src_fd);
      close(dest_fd);
      errno=tmp;
      return -1;
    }
  }

  close(src_fd);
  close(dest_fd);
  return 0;
}

static bool qemu_waited_for = false;
static pid_t qemu_pid = 0;

void sigchld_handler(int signal) {
  int status;
  pid_t child = wait(&status);
  if (child == qemu_pid) {
    qemu_waited_for = true;
    if (WIFEXITED(status)) {
      int qemu_res = WEXITSTATUS(status);
      if (qemu_res) {
        fprintf(stderr, "QEMU exited abnormally: %d\n", qemu_res);
      }
    } else {
      if (WIFSIGNALED(status)) {
        fprintf(stderr, "QEMU exited on signal %d\n", WTERMSIG(status));
      }
    }
  }
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <bootable>\n", argv[0]);
    exit(1);
  }

  const char* bootable = argv[1];

  std::string scriptDir = findScriptDir(argv[0]);
  std::string toplevel = parentOf(scriptDir.c_str());
  std::string imgsDir = toplevel+"/imgs";

  char tmpImgName[MAXPATHLEN+1];
  snprintf(tmpImgName, MAXPATHLEN, "%s/tmpXXXXXX.img", imgsDir.c_str());
  tmpImgName[MAXPATHLEN]=0;
  int tmpImgHandle = mkstemps(tmpImgName, 4);
  close(tmpImgHandle);

  std::string floppyImg = imgsDir+"/floppy.img";
  std::string hdImg = imgsDir+"/hd-grub.img";

  if (cp(floppyImg.c_str(), tmpImgName) < 0) {
    perror("cp");
    exit(7);
  }

  pid_t e2cp_pid = fork();
  if (e2cp_pid == 0) {
    // we're in the child process
    std::string kernelPath = tmpImgName;
    kernelPath += ":/kernel";
    std::cout << "e2cp " << bootable << " " << kernelPath << "\n";
    execlp("e2cp", "e2cp", bootable, kernelPath.c_str(), NULL);
    // If we get here, execlp failed.
    perror("execlp(e2cp)");
    exit(2);
  } else {
    if (e2cp_pid < 0) {
      perror("fork(e2cp)");
      unlink(tmpImgName);
      exit(9);
    }

    int status;
    pid_t p=waitpid(e2cp_pid, &status, 0);
    if (p<0) {
      perror("waitpid");
      unlink(tmpImgName);
      exit(3);
    }

    if (!(WIFEXITED(status) && (WEXITSTATUS(status)==0))) {
      if (WIFEXITED(status)) {
        fprintf(stderr,"e2cp failed: exit code %d\n", WEXITSTATUS(status));
        unlink(tmpImgName);
        exit(4);
      }

      if (WIFSIGNALED(status)) {
        fprintf(stderr,"e2cp exited on signal %d\n", WTERMSIG(status));
        unlink(tmpImgName);
        exit(5);
      }

      fprintf(stderr,"e2cp exited some other way.\n");
      unlink(tmpImgName);
      exit(6);
    }
  }

  int qemu_pipe[2];
  if (pipe(qemu_pipe) < 0) {
    perror("pipe");
    unlink(tmpImgName);
    exit(8);
  }

  qemu_pid = fork();
  if (qemu_pid == 0) {
    dup2(qemu_pipe[1], 1);
    close(qemu_pipe[0]);
    execlp("qemu-system-i386","qemu-system-i386",
           "-s",
           "-d","int,pcall",
           "-fda", tmpImgName,
           "-hda", hdImg.c_str(),
           "-boot","c",
           "-debugcon","stdio",
           NULL);
    // If we get here, execlp failed.
    perror("execlp(qemu-system-i386)");
    exit(2);
  } else {
    close(qemu_pipe[1]);
    if (qemu_pid < 0) {
      perror("fork(qemu-system-i386)");
      unlink(tmpImgName);
      exit(10);
    }

    signal(SIGCHLD, sigchld_handler);
    typedef enum {
      StartOfLine = 0,
      QAtStart = 1,
      QColonAtStart_SkipRestOfInput = 2,
      Otherwise_SkipRestOfLine = 3
    } State;

    State state = StartOfLine;

    while (true) {
      char ch;
      int nbytes = read(qemu_pipe[0], &ch, 1);
      if (nbytes < 0) {
        if (errno == EINTR) {
          continue;
        }

        int tmp = errno;
        perror("read(qemu_pipe)");
        while (!qemu_waited_for) {
          pause();
        }
      }

      if (nbytes == 0) { // eof
        while (!qemu_waited_for) {
          pause();
        }
        break;
      }

      switch (state) {
      case StartOfLine:
        if (ch=='Q') {
          state = QAtStart;
        } else {
          state = Otherwise_SkipRestOfLine;
        }
        break;

      case QAtStart:
        if (ch==':') {
          state = QColonAtStart_SkipRestOfInput;
          kill(qemu_pid, SIGINT);
        } else {
          state = Otherwise_SkipRestOfLine;
        }
        break;

      case QColonAtStart_SkipRestOfInput:
        break;

      case Otherwise_SkipRestOfLine:
        if (ch=='\n') {
          state = StartOfLine;
        }
      }
    }
  }

  unlink(tmpImgName);
  exit(11);
}
