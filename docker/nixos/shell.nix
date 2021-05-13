{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell {
  nativeBuildInputs = [ php74.unwrapped autoconf pkgconfig re2c ];
  buildInputs = [ imagemagick7 pcre2 ];

  # https://github.com/Imagick/imagick/issues/398
  FONTCONFIG_FILE = "${pkgs.fontconfig.out}/etc/fonts/fonts.conf";
}
