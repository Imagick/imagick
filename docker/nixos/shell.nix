{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell {
  nativeBuildInputs = [ php74.unwrapped autoconf pkgconfig re2c ];
  buildInputs = [ imagemagick7 pcre2 ];

  # Needed to avoid error message when using fonts
  # https://github.com/Imagick/imagick/issues/398
  FONTCONFIG_FILE = "${pkgs.fontconfig.out}/etc/fonts/fonts.conf";

  # Needed to allow Imagick::getConfigureOptions() to have all info
  # https://github.com/Imagick/imagick/issues/399
  MAGICK_CONFIGURE_PATH = "${imagemagick7.dev}/lib/ImageMagick-7.0.11/config-Q16HDRI";
}
