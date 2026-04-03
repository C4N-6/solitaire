{
  pkgs ? import <nixpkgs> { },
}:

pkgs.stdenv.mkDerivation rec {
  pname = "Solitair";
  version = "4.0.0";

  src = pkgs.fetchFromGitHub {
    owner = "C4N-6";
    repo = "solitaire";
    tag = "v${version}";
    hash = "sha256-XCW3RPz6Ui0m1NGWjPQ2lSnMtPN6pYBnTzxRFKlxWp0=";
  };

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
