#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char *progname)
{
  fprintf(stderr,  "Usage : %s bmp_file [text_file]\n", progname);
  exit(1);
}

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

int fileheader(FILE * fp, long *offset)
{
  int count = 0;
  long var_long;
  short var_short;
  char s[10];

  // BITMAP �F������ "BM"
  if (fread(s, 2, 1, fp) == 1) {
    if (memcmp(s, "BM", 2) == 0) {
      printf("[BM] BITMAP file\n");
    } 
    else {
      fprintf(stderr, "%s : Not a BITMAP file\n", s);
      exit(1);
    }
    count += 2;
  }
  printf("  [BITMAPFILEHEADER]\n");
  // �t�@�C���T�C�Y
  if (fread(&var_long, 4, 1, fp) == 1) {
    printf("  Size          : %ld [Byte]\n", var_long);
    count += 4;
  }
  // �\��̈� 0
  if (fread(&var_short, 2, 1, fp) == 1) {
    count += 2;
  }
  // �\��̈� 0
  if (fread(&var_short, 2, 1, fp) == 1) {
    count += 2;
  }
  // �t�@�C���̐擪����摜�f�[�^�܂ł̈ʒu
  if (fread(&var_long, 4, 1, fp) == 1) {
    printf("  OffBits       : %ld [Byte]\n", var_long);
    *offset = var_long;
    count += 4;
  }
  return count;
}

int infoheader(FILE * fp, short *infosize, long *width, long *height, long *x_coodinate, long *y_coodinate, short *BitCount, long *ClrUsed)
{
  int count = 0;
  long var_long, compress = 0;
  short var_short;

  // BITMAPINFOHEADER �̃T�C�Y
  if (fread(&var_long, 4, 1, fp) == 1) {
    count += 4;
    *infosize = var_long;
  }

  printf("  [BITMAPINFOHEADER]\n");
  // OS/2 Bitmap
  if (*infosize == 12) {
    // �摜�f�[�^�̕�
    if (fread(&var_short, 2, 1, fp) == 1) {
      printf("  Width         : %d [pixel]\n", var_short);
      *width = var_short;
      count += 2;
    }
    // �摜�f�[�^�̍��� 
    if (fread(&var_short, 2, 1, fp) == 1) {
      printf("  Height        : %d [pixel]\n", var_short);
      *height = var_short;
      count += 2;
    }
    // �v���[���� (1�̂�) 
    if (fread(&var_short, 2, 1, fp) == 1) {
      count += 2;
    }
    // 1��f������̃r�b�g�� (1, 4, 8, 24, 32) 
    if (fread(&var_short, 2, 1, fp) == 1) {
      printf("  BitCount      : %d [bit]\n", var_short);
      *BitCount = var_short;
      count += 2;
    }
  }
  // Windows BMP 
  else if (*infosize == 40) {
    // �摜�f�[�^�̕� 
    if (fread(&var_long, 4, 1, fp) == 1) {
      printf("  Width         : %ld [pixel]\n", var_long);
      *width = var_long;
      count += 4;
    }
    // �摜�f�[�^�̍���
    if (fread(&var_long, 4, 1, fp) == 1) {
      printf("  Height        : %ld [pixel]\n", var_long);
      *height = var_long;
      count += 4;
    }
    // �v���[���� (1�̂�) 
    if (fread(&var_short, 2, 1, fp) == 1) {
      count += 2;
    }
    // 1��f������̃r�b�g�� (1, 4, 8, 24, 32) 
    if (fread(&var_short, 2, 1, fp) == 1) {
      printf("  BitCount      : %d [bit]\n", var_short);
      *BitCount = var_short;
      count += 2;
    }
    // ���k����  0 : �����k 
    //           1 : BI_RLE8 8bit RunLength ���k 
    //           2 : BI_RLE4 4bit RunLength ���k 
    if (fread(&var_long, 4, 1, fp) == 1) {
      printf("  Compression   : %ld\n", var_long);
      compress = var_long;
      count += 4;
    }
    // �摜�f�[�^�̃T�C�Y 
    if (fread(&var_long, 4, 1, fp) == 1) {
      printf("  SizeImage     : %ld [Byte]\n", var_long);
      count += 4;
    }
    // �������𑜓x (Pixel/meter) 
    if (fread(&var_long, 4, 1, fp) == 1) {
      printf("  XPelsPerMeter : %ld [pixel/m]\n", var_long);
      *x_coodinate = var_long;
      count += 4;
    }
    // �c�����𑜓x (Pixel/meter) 
    if (fread(&var_long, 4, 1, fp) == 1) {
      printf("  YPelsPerMeter : %ld [pixel/m]\n", var_long);
      *y_coodinate = var_long;
      count += 4;
    }
    // �g�p�F�� 
    if (fread(&var_long, 4, 1, fp) == 1) {
      printf("  ClrUsed       : %ld [color]\n", var_long);
      *ClrUsed = var_long;
      count += 4;
    }
    // �d�v�ȐF�̐� 0�̏ꍇ���ׂĂ̐F 
    if (fread(&var_long, 4, 1, fp) == 1) {
      count += 4;
    }
  } 
  else {
    fprintf(stderr, "Bitmap Info Header error\n");
  }

  if (compress != 0) {
    fprintf(stderr, "���k�r�b�g�}�b�v�ɂ͑Ή����Ă��܂���\n");
    exit(1);
  }
  if (*BitCount == 4 || *BitCount == 8 || *BitCount == 24 || *BitCount == 32) {
    ;
  } 
  else {
    fprintf(stderr, "%d �r�b�g�F�ɂ͑Ή����Ă��܂���\n", *BitCount);
    exit(1);
  }
  return count;
}

/*------------------*/
/* �g�p�F���J�E���g */
/*------------------*/
long usedcolor(short bits, long color)
{
  if (color != 0) {
    return color;
  } 
  else {
    return 1 << bits;
  }
}

/*-------------*/
/* OS/2 bitmap */
/*-------------*/
int rgbtriple(FILE * fp,
              long used,
              unsigned char *red,
              unsigned char *green, 
              unsigned char *blue)
{
  long i;
  int count = 0;

  // �r�b�g�̕��т� B G R 
  for (i = 0; i < used; i++) {
    blue[i] = fgetc(fp);
    green[i] = fgetc(fp);
    red[i] = fgetc(fp);
    count++;
  }

  return count;
}

/*----------------*/
/* Windows bitmap */
/*----------------*/
int rgbquad(FILE * fp, long used, unsigned char *red,  unsigned char *green,  unsigned char *blue)
{
  long i;
  int receive, count = 0;

  // �r�b�g�̕��т� B G R �\�� 
  for (i = 0; i < used; i++) {
    blue[i] = fgetc(fp);
    green[i] = fgetc(fp);
    red[i] = fgetc(fp);
    receive = fgetc(fp);
    count++;
  }
  return count;
}

/*----------------*/
/* �e�L�X�g�_���v */
/*----------------*/
long dump32(FILE * bmp, FILE * txt, long offset, long width, long height, short bits, unsigned char *red, unsigned char *green, unsigned char *blue)
{
  int r, g, b, alpha;
  long i, j, line, position, count = 0;

  // 4byte ���E�ɂ��킹�� 
  line = (width * bits) / 8;
  if ((line % 4) != 0) {
    line = ((line / 4) + 1) * 4;
  }

  for (i = 0; i < height; i++) {
    // �s�̐擪�ʒu 
    position = offset + line * (height - (i + 1));
    fseek(bmp, position, SEEK_SET);

    // �l�̓ǂݍ��� 
    for (j = 0; j < width; j++) {
      // 32 bit �F 
      if (bits == 32) {
        b = fgetc(bmp);
        g = fgetc(bmp);
        r = fgetc(bmp);
        alpha = fgetc(bmp);
        fprintf(txt, "%d\t%d\t%d\n", r, g, b);
        count++;
      }
      // 24 bit �F 
      else if (bits == 24) {
        b = fgetc(bmp);
        g = fgetc(bmp);
        r = fgetc(bmp);
        fprintf(txt, "%d\t%d\t%d\n", r, g, b);
        count++;
      }
      // 8 bit �F 
      else if (bits == 8) {
        int index;

        index = fgetc(bmp);
        fprintf(txt, "%d\t%d\t%d\n", red[index], green[index], blue[index]);
        count++;
      }
    }
    fprintf(txt, "\n");
  }
  return count;
}

long dump4(FILE * bmp,
           FILE * txt,
           long offset,
           long width,
           long height,
           short bits,
           unsigned char *red, 
           unsigned char *green, 
           unsigned char *blue)
{
  long i, j, line, position, count = 0;

  // 4byte ���E�ɂ��킹�� 
  line = (width * bits) / 8;
  if ((line % 4) != 0) {
    line = ((line / 4) + 1) * 4;
  }

  for (i = 0; i < height; i++) {
    // �s�̐擪�ʒu 
    position = offset + line * (height - (i + 1));
    fseek(bmp, position, SEEK_SET);
    // �l�̓ǂݍ��� 
    for (j = 0; j < (int) ((double) width / 2.0 + 0.5); j++) {
      int index, high, low;

      index = fgetc(bmp);
      // ��� 4bit 
      high = index >> 4;
      // ���� 4bit 
      low = index & 15;
      fprintf(txt, "%d\t%d\t%d\n%d\t%d\t%d\n",
              red[high], green[high], blue[high],
              red[low], green[low], blue[low]);
      count += 2;
    }
    fprintf(txt, "\n");
  }
  return count;
}

/*------------*/
/* �_���v���� */
/*------------*/
void dumpprocess(char *bmpfile, char *textfile)
{
  short infosize, bits;
  long used = 0, color = 0, offset, width, height, xreso, yreso;
  unsigned char red[256], green[256], blue[256];
  FILE *fp1, *fp2;

  if ((fp1 = fopen(bmpfile, "rb")) == NULL) {
    fprintf(stderr, "Cannot open file : %s\n", bmpfile);
    exit(1);
  }
  // �w�b�_��� BITMAPFILEHEADER 
  fileheader(fp1, &offset);
  // �w�b�_��� BITMAPINFOHEADER 
  infoheader(fp1, &infosize, &width, &height, &xreso, &yreso, &bits, &color);
  color = usedcolor(bits, color);

  // OS/2 Bitmap 
  if (infosize == 12) {
    if (bits == 1 || bits == 4 || bits == 8) {
      used = rgbtriple(fp1, color, red, green, blue);
    }
    printf("[OS/2 bitmap] --- %d bit %ld color\n", bits, used);
  }
  // Windows Bitmap 
  else if (infosize == 40) {
    if (bits == 1 || bits == 4 || bits == 8) {
      used = rgbquad(fp1, color, red, green, blue);
    }
    printf("[Windows bitmap] --- %d bit %ld color\n", bits, used);
  } 
  else if (infosize == 108) {
    printf("[other bitmap]\n");
  }
  else {
    fprintf(stderr, "BITMAP INFOHEADER error\n");
  }

  if ((fp2 = fopen(textfile, "w")) == NULL) {
    fprintf(stderr, "Cannot open file : %s\n", textfile);
    exit(1);
  }
  // �e�L�X�g�o�� 
  if (bits == 32 || bits == 24 || bits == 8) {
    printf("%ld [pixels]\n",
           dump32(fp1, fp2, offset, width, height, bits, red, green, blue));
  }
  else if (bits == 4) {
    printf("%ld [pixels]\n",
           dump4(fp1, fp2, offset, width, height, bits, red, green, blue));
  }
  else {
    fprintf(stderr, "%d bit �F�ɂ͑Ή����Ă��܂���\n", bits);
    exit(1);
  }

  fclose(fp1);
  fclose(fp2);
}

/**************/
/* ���C���֐� */
/**************/
int main(int argc, char *argv[])
{
  char txtfile[255], bmpfile[255];

  if (argc < 2) {
    usage(argv[0]);
  }
  else if (argc == 2) {
    char basename[255];

    strcpy(bmpfile, argv[1]);
    getbasename(basename, argv[1]);
    sprintf(txtfile, "%s.txt", basename);
  } 
  else if (argc == 3) {
    strcpy(bmpfile, argv[1]);
    strcpy(txtfile, argv[2]);
  } 
  else {
    usage(argv[0]);
  }

  dumpprocess(bmpfile, txtfile);

  return 0;
}
