myCppLib

kentarou06's c++ library.


https://github.com/kentarou06/myCppLib

git clone https://github.com/kentarou06/myCppLib.git ${作成するディレクトリ}


# 実行ファイル・共有ライブラリ　の検索場所の指定

#  for zsh  ( .zshrc )
LD_LIBRARY_PATH=.:${HOME}/myCppLib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

#  for bash ( .bashrc )
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/myCppLib
export  LD_LIBRARY_PATH
