
//参考元：
//http://zarb.org/~gc/html/libpng.html


#include <iostream>

#include <png.h>

//エラーの時強制終了
void abort_(const char* c) {
  printf(c);
  abort();
}

//! @brief pngファイル読み込み関数
//! @param [in] file_name ファイル名
//! @param [out] width 画像幅（ピクセル）
//! @param [out] height 画像高さ（ピクセル）
//! @param [out] color_type RGBかRGBAか...等
//! @param [out] bit_depth チャンネルのビット数
//! @param [out] row_pointers 画像データへのポインタのポインタ
void read_png(
  const char* file_name,
  int* width,
  int* height,
  png_byte* color_type,
  png_byte* bit_depth,
  png_bytep** row_pointers

) {


  png_byte  header[8];    // 8 is the maximum size that can be checked

  std::cout << file_name << std::endl;
  FILE *fp = fopen(file_name, "rb");
  if (!fp) {
    abort_("[read_png_file] File could not be opened for reading");
  }

  fread(header, 1, 8, fp);
  int is_png = !png_sig_cmp(header, 0, 8);

  if (!is_png)
  {
    std::cout << "not png" << std::endl;
    return;
  }
  
  png_structp png_ptr;
  png_infop info_ptr;

  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  std::cout << "debug1" << std::endl;
  
  if (!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");
  
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");
  
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);
  
  png_read_info(png_ptr, info_ptr);

      

  /////////////////////////////////////////
  // 画像情報の取得
  *width = png_get_image_width(png_ptr, info_ptr);
  *height = png_get_image_height(png_ptr, info_ptr);
  *color_type = png_get_color_type(png_ptr, info_ptr);
  *bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  //
  /////////////////////////////////////////

  int number_of_passes;
  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);
  
  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during read_image");
  

  /////////////////////////////////////////
  // 画像の読み込み
  *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * *height);

  for (int y=0; y < *height; y++)
    (*row_pointers)[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
  
  png_read_image(png_ptr, *row_pointers);
  //
  /////////////////////////////////////////

  
  fclose(fp);
}

int main()
{
  int width;
  int height;
  png_byte color_type;
  png_byte bit_depth;

  png_bytep* row_pointers;

  ////////////////////////////////
  // 画像読み込み
  read_png(
    "image.png",
    &width,
    &height,
    &color_type,
    &bit_depth,
    &row_pointers
  );
  //
  ////////////////////////////////

  std::cout << "width : " << width << std::endl;
  std::cout << "height: " << height << std::endl;
  std::cout << "colortype: " << (int)color_type << std::endl;
  std::cout << "bitdepth: " << (int)bit_depth << std::endl;

  ////////////////////////////////
  //１ピクセルのバイト数を算出
  //RGBかRGBAだけ出力
  int pixelsize;
  switch (color_type){
  case PNG_COLOR_TYPE_RGB:
    pixelsize = bit_depth / 8 * 3;
    break;
  case PNG_COLOR_TYPE_RGB_ALPHA:
    pixelsize = bit_depth / 8 * 4;
    break;
  default:
    std::cout << "RGB/RGBA only" << std::endl;
  }

  return 0;
}

