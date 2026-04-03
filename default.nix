{
  pkgs ? import <nixpkgs> { },
}:

pkgs.stdenv.mkDerivation rec {
  pname = "Solitair";
  version = "4.1.0";

  src = ./.;

  nativeBuildInputs = [
    pkgs.cmake
    pkgs.gcc
    pkgs.pkg-config
  ];

  buildInputs = [
    pkgs.nlohmann_json
    pkgs.argparse
    pkgs.readline
  ];

  meta = {
    description = "a solitaire game";
    homepage = "https://github.com/C4N-6/solitaire";
  };
}
