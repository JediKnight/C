/***************************************************************/
/* �f�[�^ �� �摜�ϊ��\�t�g                                    */
/* �o�͉摜�t�H�[�}�b�g: bitmap �`��                           */
/*                                                             */
/* �f�[�^�t�H�[�}�b�g                                          */
/* gnuplot ��                                                  */
/* set data style lines                                        */
/* splot �ŕ\���ł���`��                                      */
/*                                                             */
/* ("%f %f %f\n", x, y, z)                                     */
/* ("\n")                                                      */
/*                                                             */
/* kondo@kk.iij4u.or.jp                                        */
/* 2000.08.03                                                  */
/***************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BITCOUNT (8)
#define BUF_SIZE (1000)
#ifndef _MAX_PATH
#define _MAX_PATH (255)
#endif
#ifndef UCHAR_MAX
#define UCHAR_MAX (255)
#endif
#ifndef UCHAR_MIN
#define UCHAR_MIN (0)
#endif

static char cmdname[] = "txt2bmp";

int optind = 1; // �I�v�V�����łȂ��ŏ��̈����܂ł̕�����
int optopt;     // �I�v�V�������� 
char *optarg;   // �I�v�V��������

double max = 0.0, min = 0.0;
int maxopt = 0, minopt = 0;

void usage(void)
{
  fprintf(stderr,
          "\n"
          "ASCII�e�L�X�g�f�[�^ ---> BMP �t�H�[�}�b�g�ϊ��v���O����\n"
          "usage : %s [option] <text_file> [bitmap_file]\n"
          "  bitmap_file �̎w�肪�����ꍇ"
          " text_file.bmp �̃t�@�C�����쐬����܂�\n"
          "\n"
          "option :\n"
          "  -M : �ő�l���w�肵�܂�\n"
          "  -m : �ŏ��l���w�肵�܂�\n"
          "\n"
          "�f�[�^�`�� : X���W Y���W Z�l\n"
          "\tx y z\n"
          "\tx y z\n"
          "\t: : :\n"
          "\t      (���s�݂̂Ń��C���̋�؂�)\n"
          "\tx y z\n"
          "\t: : :\n"
          "\n", cmdname);
  exit(1);
}

void openerror(char *filename)
{
  fprintf(stderr, "Can't open file: %s\n", filename);
  exit(1);
}

void allocerror(void)
{
  fprintf(stderr, "memory allocation error.\n");
  exit(1);
}

/*----------------*/
/* �t�@�C�����擾 */
/*----------------*/
int getbasename(char *dest, char *src)
{
  int i, start, end, ret;

  i = -1;
  start = 0;
  end = 0;

  // �t�@�C�����̂͂��߂ƏI�������o 
  while (src[++i]) {
    if (src[i] == '\\' || src[i] == ':') {
      start = i + 1;
      end = 0;
    }
    if (src[i] == '.') {
      end = i;
    }
  }
  if (end == 0) {
    end = i;
  }

  // �t�@�C�������L��ꍇ 
  if (start < end) {
    for (i = 0; i < end; i++) {
      dest[i] = src[i];
    }
    dest[i] = '\0';
    ret = 1;
  }
  else {
    dest[0] = '\0';
    ret = 0;
  }

  return ret;
}

/*----------------------*/
/* �R�����g�s�ǂݔ�΂� */
/*----------------------*/
void commentskip(FILE * fp)
{
  int c;

  do {
    while ((c = fgetc(fp)) == '\n') {
      ;
    }
    // # ; / ������Γǂݔ�΂� 
    if ((c == '#') || (c == ';') || (c == '/')) {
      while ((c = fgetc(fp)) != '\n') {
        ;
      }
    }
    else {
      break;
    }
  } while (1);

  ungetc(c, fp);
}

/*------------------*/
/* �ő�l�ŏ��l���� */
/*------------------*/
void maxmin(FILE * fp,
            double *maxx, double *minx,
            double *maxy, double *miny,
            int *sx, int *sy)
{
  int n, cx, cy, maxcx, flag;
  double x, y, z, xrange, yrange, xunit, yunit;
  char s[2], buf[BUF_SIZE];

  cx = 0;                       // x �����̃f�[�^�� 
  cy = 0;                       // y �����̃f�[�^�� 
  maxcx = 0;                    // x �����̃f�[�^���ő�l 
  flag = 0;

  rewind(fp);
  commentskip(fp);

  // �l�̏����� 
  if (fgets(buf, BUF_SIZE, fp) != NULL) {
    n = sscanf(buf, "%lf %lf %lf %1s", &x, &y, &z, s);
    if (n >= 3) {
      *maxx = x;
      *minx = x;
      *maxy = y;
      *miny = y;
      if (maxopt == 0) {
        max = z;
      }
      if (minopt == 0) {
        min = z;
      }
    }
  }

  rewind(fp);

  while (fgets(buf, BUF_SIZE, fp) != NULL) {
    n = sscanf(buf, "%lf %lf %lf %1s", &x, &y, &z, s);

    // EOF �ŏI�� 
    if (feof(fp)) {
      break;
    }
    // �f�[�^��3�ȏ�̏ꍇ 
    else if (n >= 3) {
      cx++;
      if (flag == 0) {
        cy++;
      }
      flag = 1;

      // �ő�l���� 
      if (z < min) {
        if (minopt == 0) {
          min = z;
    }
      }
      if (z > max) {
        if (maxopt == 0) {
          max = z;
    }
      }

      if (x < *minx) {
        *minx = x;
      }
      else if (x > *maxx) {
        *maxx = x;
      }

      if (y < *miny) {
        *miny = y;
      }
      else if (y > *maxy) {
        *maxy = y;
      }
    }
    // ���s�݂̂̏ꍇ 
    else {
      if (flag == 1) {
        if (cx > maxcx) {
          maxcx = cx;
        }
      }

      flag = 0;
      cx = 0;
    }
  }

  // �f�[�^�͈� 
  xrange = *maxx - *minx;
  yrange = *maxy - *miny;

  printf("Horizontal : min = %g, max = %g, range = %g\n",
         *minx, *maxx, xrange);
  printf("Vertical   : min = %g, max = %g, range = %g\n",
         *miny, *maxy, yrange);
  printf("Data       : min = %g, max = %g\n", min, max);

  if (max <= min) {
    fprintf(stderr, "�ő�l�����ŏ��l���傫��\n");
    exit(1);
  }

  // 1��f������̒P�� 
  xunit = xrange / maxcx;
  yunit = yrange / cy;

  // ��f�� 
  *sx = (int) (xrange / xunit);
  *sy = (int) (yrange / yunit);
}

/*----------------*/
/* �f�[�^�ǂݍ��� */
/*----------------*/
void dataread(FILE * fp,
              unsigned char **data,
              double maxx, double minx,
              double maxy, double miny,
              int sx, int sy)
{
  int n;
  double x, y, z,
         xrange, yrange, zrange,
         value;
  char s[2], buf[BUF_SIZE];

  xrange = maxx - minx;
  yrange = maxy - miny;
  zrange = max - min;

  rewind(fp);
  commentskip(fp);

  while (fgets(buf, BUF_SIZE, fp) != NULL) {
    n = sscanf(buf, "%lf %lf %lf %1s", &x, &y, &z, s);

    if (feof(fp)) {
      break;
    }
    else if (n >= 3) {
      int ix, iy;

      // ���W�ϊ� 
      ix = (int) (0.5 + (x - minx) * (double) (sx - 1) / xrange);
      iy = (int) (0.5 + (y - miny) * (double) (sy - 1) / yrange);

      value = (unsigned char) (255.9 * (z - min) / zrange);
      if (value > UCHAR_MAX) {
        value = UCHAR_MAX;
      }
      else if (value < UCHAR_MIN) {
        value = UCHAR_MIN;
      }
      data[ix][iy] = value;
    }
  }
}

/*---------------------------------*/
/* �t�@�C���w�b�_ BITMAPFILEHEADER */
/*---------------------------------*/
int fileheader(FILE * fp,
               long size, long offset)
{
  long count, filesize;
  short reserved = 0;
  char s[2];

  rewind(fp);

  // ���ʕ��� BM 
  s[0] = 'B';
  s[1] = 'M';
  fwrite(s, sizeof(char), 2, fp);

  printf("[BITMAPFILEHEADER]\n");
  // �t�@�C���T�C�Y bfSize 
  filesize = size + offset;
  printf("  File Size          = %ld[Bytes]\n", filesize);
  fwrite(&filesize, sizeof(long), 1, fp);
  // �\��G���A bfReserved1 
  fwrite(&reserved, sizeof(short), 1, fp);
  // �\��G���A bfReserved2 
  fwrite(&reserved, sizeof(short), 1, fp);
  // �f�[�^���܂ł̃I�t�Z�b�g bfOffBits 
  printf("  Bitmap Data Offset = %ld [Bytes]\n", offset);
  fwrite(&offset, sizeof(long), 1, fp);

  // �t�@�C���w�b�_�T�C�Y 14 Byte 
  if ((count = ftell(fp)) != 14) {
    fprintf(stderr, "BITMAPFILEHEADER write error : %ld\n", count);
    exit(1);
  }

  return count;
}

/*-----------------------------*/
/* ���w�b�_ BITMAPINFOHEADER */
/*-----------------------------*/
int bitmapheader(FILE * fp,
                 long width, long height, long size)
{
  long count, var_long;
  short var_short;

  fseek(fp, 14, SEEK_SET);

  printf("[BITMAPINFOHEADER]\n");
  // ���w�b�_�̃T�C�Y biSize (Windows BMP �� 40) 
  var_long = 40;
  fwrite(&var_long, sizeof(long), 1, fp);
  // �摜�̕� biWidth 
  var_long = width;
  printf("  Width              = %ld [pixels]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // �摜�̍��� biHeight 
  // (�����Ȃ�΍�������E��, �}�C�i�X�Ȃ�΍��ォ��E��) 
  var_long = height;
  printf("  Height             = %ld [pixels]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // �v���[���� biPlanes (�K�� 1) 
  var_short = 1;
  printf("  Planes             = %hd\n", var_short);
  fwrite(&var_short, sizeof(short), 1, fp);
  // 1�s�N�Z���̃f�[�^�� biBitCount (1, 4, 8, 24, 32) 
  var_short = BITCOUNT;
  printf("  Bits Per Pixel     = %hd [bits]\n", var_short);
  fwrite(&var_short, sizeof(short), 1, fp);

  // ���k biCompression (�����k�Ȃ�� 0) 
  var_long = 0;
  printf("  Compression        = %ld\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // �摜�̃T�C�Y biSizeImage 
  var_long = size;
  printf("  Bitmap Data Size   = %ld [Bytes]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // �������𑜓x pixel/m biXPelPerMeter 
  // (96dpi, 1inch = 0.0254m �̂Ƃ� 96/0.0254 = 3780) 
  var_long = 3780;
  printf("  HResolution        = %ld [pixel/m]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // �c�����𑜓x pixel/m biYPelPerMeter 
  // (96dpi, 1inch = 0.0254m �̂Ƃ� 96/0.0254 = 3780) 
  var_long = 3780;
  printf("  VResolution        = %ld [pixel/m]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // �p���b�g�� biClrUsed 
  var_long = 1 << BITCOUNT;
  printf("  Colors             = %ld [colors]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // �p���b�g���̏d�v�ȐF  biClrImportant 
  var_long = 0;
  printf("  Important Colors   = %ld\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);

  // �t�@�C���w�b�_(14 Byte) + ���w�b�_�T�C�Y(40 Byte) 
  if ((count = ftell(fp)) != 54) {
    fprintf(stderr, "BITMAPINFOHEADER write error : %ld\n", count);
    exit(1);
  }

  return count;
}

/*------------------------*/
/* �p���b�g�f�[�^ RGBQUAD */
/*------------------------*/
long rgbquad(FILE * fp)
{
  long i, color, count;

  fseek(fp, 54, SEEK_SET);

  color = 1 << BITCOUNT;

  printf("  Pallete            = %ld [colors] (%ld [Bytes])\n",
         color, 4 * color);
  for (i = 0; i < color; i++) {
    unsigned char red, green, blue, reserved = 0;

    // �O���C�X�P�[�� red = green = blue 
    blue = (unsigned char) i;
    green = (unsigned char) i;
    red = (unsigned char) i;

    fputc(blue, fp);
    fputc(green, fp);
    fputc(red, fp);
    fputc(reserved, fp);
  }

  if ((count = ftell(fp)) != 54 + 4 * color) {
    fprintf(stderr, "RGBQUAD write error : %ld\n", count);
    exit(1);
  }

  return count;
}

/*------------*/
/* �摜�f�[�^ */
/*------------*/
long imagedata(FILE * fp,
               int x, int y,
               unsigned char **data,
               long offset)
{
  long i, j;
  unsigned char index;

  fseek(fp, offset, SEEK_SET);

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      index = data[i][j];
      fwrite(&index, sizeof(char), 1, fp);
    }
    // 1���C���� 4Byte(long) ���E�ɂ��킹�� 
    if ((x % 4) != 0) {
      for (i = 0; i < ((x / 4 + 1) * 4) - x; i++) {
        unsigned char zero = 0;

        fwrite(&zero, sizeof(char), 1, fp);
      }
    }
  }

  return ftell(fp);
}

/*-----------------*/
/* BITMAP �������� */
/*-----------------*/
void bmpwrite(char *txtfile, char *bmpfile)
{
  int i, j, sx, sy;
  long size, offset;
  double maxx, minx, maxy, miny;
  unsigned char **data;
  FILE *fp1, *fp2;

  // �e�L�X�g�t�@�C�� 
  if ((fp1 = fopen(txtfile, "r")) == NULL) {
    openerror(txtfile);
  }

  // �ő�l�ŏ��l���� 
  maxmin(fp1, &maxx, &minx, &maxy, &miny, &sx, &sy);

  // �������m�� 
  if ((data = malloc(sx * sizeof(char *))) == NULL) {
    allocerror();
  }
  for (i = 0; i < sx; i++) {
    if ((data[i] = malloc(sy * sizeof(char))) == NULL) {
      allocerror();
    }
    for (j = 0; j < sy; j++) {
      data[i][j] = 0;
    }
  }

  // �f�[�^�擾 
  dataread(fp1, data, maxx, minx, maxy, miny, sx, sy);

  fclose(fp1);

  if ((fp2 = fopen(bmpfile, "wb")) == NULL) {
    openerror(bmpfile);
  }

  // �摜�T�C�Y (1���C����4Byte(long)���E�ɂ��킹��) 
  if (sx % 4 != 0) {
    size = ((sx / 4 + 1) * 4) * sy;
  }
  else {
    size = sx * sy;
  }
  // �摜�f�[�^�܂ł̃I�t�Z�b�g(�w�b�_�T�C�Y) 
  offset = 14 + 40 + 4 * (1 << BITCOUNT);

  // �t�@�C���w�b�_ 
  fileheader(fp2, size, offset);
  // ���w�b�_ 
  bitmapheader(fp2, (long) sx, (long) sy, size);
  // �p���b�g�f�[�^ 
  rgbquad(fp2);
  // �摜�f�[�^ 
  imagedata(fp2, sx, sy, data, offset);

  free(data);
  fclose(fp2);
}

/*----------------------*/
/* �I�v�V���������̉�� */
/*----------------------*/
int getopt(int nargc, char **nargv, char *ostr)
{
  static char *place = "",
              *lastostr = (char *) 0;
  char *oli;

  if (ostr != lastostr) {
    lastostr = ostr;
    place = "";
  }
  if (!*place) {
    if ((optind >= nargc) 
        || (*(place = nargv[optind]) != '-')
        || !*++place) {
      place = "";
      return (EOF);
    }
    if (*place == '-') {
      ++optind;
      return (EOF);
    }
  }
  // �I�v�V���������̃`�F�b�N 
  if ((optopt = (int) *place++) == (int) ':'
      || !(oli = strchr(ostr, optopt))) {
    if (!*place) {
      ++optind;
    }

    fprintf(stderr, "%s : �I�v�V�������Ⴂ�܂� -- %c\n", *nargv, optopt);
    return (int) '?';
  }
  // �I�v�V���������̃`�F�b�N 
  if (*++oli != ':') {
    optarg = NULL;
    if (!*place) {
      ++optind;
    }
  }
  // �I�v�V�����������K�v�ȏꍇ 
  else {
    if (*place) {
      optarg = place;
    }
    // �����������ꍇ 
    else if (nargc <= ++optind) {
      place = "";
      fprintf(stderr, "%s : �I�v�V�����ɂ͈������K�v�ł� -- %c\n",
              *nargv, optopt);
      return (int) '?';
    }
    // ����������ꍇ 
    else {
      optarg = nargv[optind];
    }
    place = "";
    ++optind;
  }
  return (optopt);
}

/**************/
/* ���C���֐� */
/**************/
int main(int argc, char *argv[])
{
  int c;
  char txtfile[_MAX_PATH], bmpfile[_MAX_PATH], basename[_MAX_PATH];

  while ((c = getopt(argc, argv, "M:m:")) != EOF) {
    switch (c) {
    case 'M':                  // �ő�l 
      max = strtod(optarg, (char **) NULL);
      maxopt = 1;
      break;
    case 'm':                  // �ŏ��l 
      min = strtod(optarg, (char **) NULL);
      minopt = 1;
      break;
    case '?':
      usage();
      break;
    default:
      usage();
      break;
    }
  }

  if (argc - optind < 1) {
    usage();
  }
  else if (argc - optind == 1) {
    strcpy(txtfile, argv[optind]);
    getbasename(basename, argv[optind]);
    sprintf(bmpfile, "%s.bmp", basename);
  }
  else {
    strcpy(txtfile, argv[optind]);
    strcpy(bmpfile, argv[optind + 1]);
  }

  bmpwrite(txtfile, bmpfile);

  return 0;
}
