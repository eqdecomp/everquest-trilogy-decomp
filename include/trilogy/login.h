/* login.h - login/error-dialog entry points. */
#ifndef TRILOGY_LOGIN_H
#define TRILOGY_LOGIN_H

#include "common.h"

void showErrorDialog_guess(void* instance, void* parent, u32 title, u32 body);
int DialogFunc_guess(void* dialog, int message, short wParam);

#endif /* TRILOGY_LOGIN_H */
