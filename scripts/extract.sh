for arc in *.7z
do
  7za x "$arc"
done
mkdir data
for arc in *.tar
do
  tar -xvf "$arc" -C data/
  rm -r "$arc"
  rm -r "$arc".7z
done

