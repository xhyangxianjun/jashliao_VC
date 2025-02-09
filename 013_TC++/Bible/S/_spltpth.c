/*
_splitpath                  ― DOS    ー UNIX     ― Windows
                            ー ANSI C ー C++ only
�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w

�y�k:

      void _splitpath(const char *path,
                      char *drive, chart *dir,
                      char *name, char *ext);

委柴察��:

      const char *path;     往�Qだ狐�婚江禪��|�C
      char *drive;          栽歳�N�X�C
      char *dir;            ヘ雀�W細�C
      char *name;           隻�廢W細�C
      char *ext;            �畿�隻�W�C

�\�犖兒�:

      �宏膽踏皎匐任JЧ障侯�|だ狐Θ栽歳穣�N�Xdrive �Bヘ雀�W細
      dir �B隻�廢W細name�Bの�畿�隻�Wext �C�C�@�唸鐇垣x�s�Uだ
      狐魁だ�填・椶岫nΤ_MAX_DRIVE�B_MAX_DIR�B_MAX_FILE�B の
      _MAX_EXT�哘譴顕�, �o�捻`柴�O�w�q�鷂��J隻stdlib.hい�Cだ
      狐�畧x�s��drive �勦��V�昏填゜虻郎仂��N�X, �奪B�[�W�@��
      だ弦':' , �勁p "A:" , dir �勦��V�根阿@ヘ雀�W細, �O�愛b
      飢ЮΤは娃弦�墾r��, �勁p "\BORLAND\BIN\", τname業�x�s
      隻�廢W細, �椥�ext, �筴勦��V�困混e�哀H�y弦'.'�旭_�I�墾r
      ��, �O隻�廬懇畿�隻�W, �勁p".EXE" �C

厚�J隻:

      stdlib.h

暁�^��:

      �L�C

�枌��膽�:

      _fullpath, _makepath

�d��:

      ��パ�魯葎命蕕J�@Ч障�裟筆廚慮��|�W細, �A�Qノ_splitpath
      �膽同N�區蕕J�裟筆廚慮��|�W細だ狐Θ栽歳穣�N�X�Bヘ雀侯�|
      �B隻�W�Bの�畿�隻�W, �A�N飢�G塔ボ�鷽湛��W�C
*/
/* Program : _spltpth.c */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void main()
{
   char fullpath[_MAX_PATH];
   char drive[_MAX_DRIVE];
   char directory[_MAX_DIR];
   char filename[_MAX_FNAME];
   char extension[_MAX_EXT];

   clrscr();
   printf("Please enter a full path :\n=>");
   gets(fullpath);

   _splitpath(fullpath, drive, directory, filename, extension);

   printf("Disc drive     = %s\n", drive);
   printf("Directory      = %s\n", directory);
   printf("File name      = %s\n", filename);
   printf("File extension = %s\n", extension);

   printf("\nPress any key to continue...");
   getch();
}
