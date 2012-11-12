/***************************************************************/
/* データ → 画像変換ソフト                                    */
/* 出力画像フォーマット: bitmap 形式                           */
/*                                                             */
/* データフォーマット                                          */
/* gnuplot で                                                  */
/* set data style lines                                        */
/* splot で表示できる形式                                      */
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

int optind = 1; // オプションでない最初の引数までの文字列数
int optopt;     // オプション文字 
char *optarg;   // オプション引数

double max = 0.0, min = 0.0;
int maxopt = 0, minopt = 0;

void usage(void)
{
  fprintf(stderr,
          "\n"
          "ASCIIテキストデータ ---> BMP フォーマット変換プログラム\n"
          "usage : %s [option] <text_file> [bitmap_file]\n"
          "  bitmap_file の指定が無い場合"
          " text_file.bmp のファイルが作成されます\n"
          "\n"
          "option :\n"
          "  -M : 最大値を指定します\n"
          "  -m : 最小値を指定します\n"
          "\n"
          "データ形式 : X座標 Y座標 Z値\n"
          "\tx y z\n"
          "\tx y z\n"
          "\t: : :\n"
          "\t      (改行のみでラインの区切り)\n"
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
/* ファイル名取得 */
/*----------------*/
int getbasename(char *dest, char *src)
{
  int i, start, end, ret;

  i = -1;
  start = 0;
  end = 0;

  // ファイル名のはじめと終わりを検出 
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

  // ファイル名が有る場合 
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
/* コメント行読み飛ばし */
/*----------------------*/
void commentskip(FILE * fp)
{
  int c;

  do {
    while ((c = fgetc(fp)) == '\n') {
      ;
    }
    // # ; / があれば読み飛ばし 
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
/* 最大値最小値検索 */
/*------------------*/
void maxmin(FILE * fp,
            double *maxx, double *minx,
            double *maxy, double *miny,
            int *sx, int *sy)
{
  int n, cx, cy, maxcx, flag;
  double x, y, z, xrange, yrange, xunit, yunit;
  char s[2], buf[BUF_SIZE];

  cx = 0;                       // x 方向のデータ数 
  cy = 0;                       // y 方向のデータ数 
  maxcx = 0;                    // x 方向のデータ数最大値 
  flag = 0;

  rewind(fp);
  commentskip(fp);

  // 値の初期化 
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

    // EOF で終了 
    if (feof(fp)) {
      break;
    }
    // データ数3個以上の場合 
    else if (n >= 3) {
      cx++;
      if (flag == 0) {
        cy++;
      }
      flag = 1;

      // 最大値検索 
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
    // 改行のみの場合 
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

  // データ範囲 
  xrange = *maxx - *minx;
  yrange = *maxy - *miny;

  printf("Horizontal : min = %g, max = %g, range = %g\n",
         *minx, *maxx, xrange);
  printf("Vertical   : min = %g, max = %g, range = %g\n",
         *miny, *maxy, yrange);
  printf("Data       : min = %g, max = %g\n", min, max);

  if (max <= min) {
    fprintf(stderr, "最大値よりも最小値が大きい\n");
    exit(1);
  }

  // 1画素あたりの単位 
  xunit = xrange / maxcx;
  yunit = yrange / cy;

  // 画素数 
  *sx = (int) (xrange / xunit);
  *sy = (int) (yrange / yunit);
}

/*----------------*/
/* データ読み込み */
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

      // 座標変換 
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
/* ファイルヘッダ BITMAPFILEHEADER */
/*---------------------------------*/
int fileheader(FILE * fp,
               long size, long offset)
{
  long count, filesize;
  short reserved = 0;
  char s[2];

  rewind(fp);

  // 識別文字 BM 
  s[0] = 'B';
  s[1] = 'M';
  fwrite(s, sizeof(char), 2, fp);

  printf("[BITMAPFILEHEADER]\n");
  // ファイルサイズ bfSize 
  filesize = size + offset;
  printf("  File Size          = %ld[Bytes]\n", filesize);
  fwrite(&filesize, sizeof(long), 1, fp);
  // 予約エリア bfReserved1 
  fwrite(&reserved, sizeof(short), 1, fp);
  // 予約エリア bfReserved2 
  fwrite(&reserved, sizeof(short), 1, fp);
  // データ部までのオフセット bfOffBits 
  printf("  Bitmap Data Offset = %ld [Bytes]\n", offset);
  fwrite(&offset, sizeof(long), 1, fp);

  // ファイルヘッダサイズ 14 Byte 
  if ((count = ftell(fp)) != 14) {
    fprintf(stderr, "BITMAPFILEHEADER write error : %ld\n", count);
    exit(1);
  }

  return count;
}

/*-----------------------------*/
/* 情報ヘッダ BITMAPINFOHEADER */
/*-----------------------------*/
int bitmapheader(FILE * fp,
                 long width, long height, long size)
{
  long count, var_long;
  short var_short;

  fseek(fp, 14, SEEK_SET);

  printf("[BITMAPINFOHEADER]\n");
  // 情報ヘッダのサイズ biSize (Windows BMP は 40) 
  var_long = 40;
  fwrite(&var_long, sizeof(long), 1, fp);
  // 画像の幅 biWidth 
  var_long = width;
  printf("  Width              = %ld [pixels]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // 画像の高さ biHeight 
  // (正数ならば左下から右上, マイナスならば左上から右下) 
  var_long = height;
  printf("  Height             = %ld [pixels]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // プレーン数 biPlanes (必ず 1) 
  var_short = 1;
  printf("  Planes             = %hd\n", var_short);
  fwrite(&var_short, sizeof(short), 1, fp);
  // 1ピクセルのデータ数 biBitCount (1, 4, 8, 24, 32) 
  var_short = BITCOUNT;
  printf("  Bits Per Pixel     = %hd [bits]\n", var_short);
  fwrite(&var_short, sizeof(short), 1, fp);

  // 圧縮 biCompression (無圧縮ならば 0) 
  var_long = 0;
  printf("  Compression        = %ld\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // 画像のサイズ biSizeImage 
  var_long = size;
  printf("  Bitmap Data Size   = %ld [Bytes]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // 横方向解像度 pixel/m biXPelPerMeter 
  // (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780) 
  var_long = 3780;
  printf("  HResolution        = %ld [pixel/m]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // 縦方向解像度 pixel/m biYPelPerMeter 
  // (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780) 
  var_long = 3780;
  printf("  VResolution        = %ld [pixel/m]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // パレット数 biClrUsed 
  var_long = 1 << BITCOUNT;
  printf("  Colors             = %ld [colors]\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);
  // パレット中の重要な色  biClrImportant 
  var_long = 0;
  printf("  Important Colors   = %ld\n", var_long);
  fwrite(&var_long, sizeof(long), 1, fp);

  // ファイルヘッダ(14 Byte) + 情報ヘッダサイズ(40 Byte) 
  if ((count = ftell(fp)) != 54) {
    fprintf(stderr, "BITMAPINFOHEADER write error : %ld\n", count);
    exit(1);
  }

  return count;
}

/*------------------------*/
/* パレットデータ RGBQUAD */
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

    // グレイスケール red = green = blue 
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
/* 画像データ */
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
    // 1ラインは 4Byte(long) 境界にあわせる 
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
/* BITMAP 書き込み */
/*-----------------*/
void bmpwrite(char *txtfile, char *bmpfile)
{
  int i, j, sx, sy;
  long size, offset;
  double maxx, minx, maxy, miny;
  unsigned char **data;
  FILE *fp1, *fp2;

  // テキストファイル 
  if ((fp1 = fopen(txtfile, "r")) == NULL) {
    openerror(txtfile);
  }

  // 最大値最小値検索 
  maxmin(fp1, &maxx, &minx, &maxy, &miny, &sx, &sy);

  // メモリ確保 
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

  // データ取得 
  dataread(fp1, data, maxx, minx, maxy, miny, sx, sy);

  fclose(fp1);

  if ((fp2 = fopen(bmpfile, "wb")) == NULL) {
    openerror(bmpfile);
  }

  // 画像サイズ (1ラインは4Byte(long)境界にあわせる) 
  if (sx % 4 != 0) {
    size = ((sx / 4 + 1) * 4) * sy;
  }
  else {
    size = sx * sy;
  }
  // 画像データまでのオフセット(ヘッダサイズ) 
  offset = 14 + 40 + 4 * (1 << BITCOUNT);

  // ファイルヘッダ 
  fileheader(fp2, size, offset);
  // 情報ヘッダ 
  bitmapheader(fp2, (long) sx, (long) sy, size);
  // パレットデータ 
  rgbquad(fp2);
  // 画像データ 
  imagedata(fp2, sx, sy, data, offset);

  free(data);
  fclose(fp2);
}

/*----------------------*/
/* オプション引数の解析 */
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
  // オプション文字のチェック 
  if ((optopt = (int) *place++) == (int) ':'
      || !(oli = strchr(ostr, optopt))) {
    if (!*place) {
      ++optind;
    }

    fprintf(stderr, "%s : オプションが違います -- %c\n", *nargv, optopt);
    return (int) '?';
  }
  // オプション引数のチェック 
  if (*++oli != ':') {
    optarg = NULL;
    if (!*place) {
      ++optind;
    }
  }
  // オプション引数が必要な場合 
  else {
    if (*place) {
      optarg = place;
    }
    // 引数が無い場合 
    else if (nargc <= ++optind) {
      place = "";
      fprintf(stderr, "%s : オプションには引数が必要です -- %c\n",
              *nargv, optopt);
      return (int) '?';
    }
    // 引数がある場合 
    else {
      optarg = nargv[optind];
    }
    place = "";
    ++optind;
  }
  return (optopt);
}

/**************/
/* メイン関数 */
/**************/
int main(int argc, char *argv[])
{
  int c;
  char txtfile[_MAX_PATH], bmpfile[_MAX_PATH], basename[_MAX_PATH];

  while ((c = getopt(argc, argv, "M:m:")) != EOF) {
    switch (c) {
    case 'M':                  // 最大値 
      max = strtod(optarg, (char **) NULL);
      maxopt = 1;
      break;
    case 'm':                  // 最小値 
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
