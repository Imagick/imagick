{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell {
  nativeBuildInputs = [ php74.unwrapped autoconf pkgconfig re2c ];
  buildInputs = [ imagemagick7 pcre2 ];
}
