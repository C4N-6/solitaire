// UserName.h
#ifndef USERNAME_H
#define USERNAME_H

#include <string>

#ifdef _WIN32 // For Windows
#include <Lmcons.h>
#include <windows.h>

inline std::string getCurrentUserName() {
  char username[UNLEN + 1];
  DWORD username_len = UNLEN + 1;
  if (GetUserNameA(username, &username_len)) {
    return std::string(username);
  } else {
    return "Unknown User";
  }
}

#elif __linux__ // For Linux
#include <pwd.h>
#include <unistd.h>

inline std::string getCurrentUserName() {
  struct passwd *pw = getpwuid(getuid());
  if (pw) {
    return std::string(pw->pw_name);
  } else {
    return "Unknown User";
  }
}

#elif __APPLE__ // For macOS
#include <pwd.h>
#include <unistd.h>

inline std::string getCurrentUserName() {
  struct passwd *pw = getpwuid(getuid());
  if (pw) {
    return std::string(pw->pw_name);
  } else {
    return "Unknown User";
  }
}
#else

inline std::string getCurrentUserName() { return "Unknown User"; }

#endif // End of OS checks

#endif // USERNAME_H
