#!/bin/bash

set -eu
cd `dirname $0`

A_URL=$1

[[ -e third_party_tmp ]] && echo "third_party_tmp exists. abort!" && exit 1

mkdir third_party_tmp
pushd third_party_tmp > /dev/null

wget $A_URL
ZIP_FILENAME=`ls`

unzip $ZIP_FILENAME
rm $ZIP_FILENAME
TOOL_NAME=`ls`
echo $TOOL_NAME
TOOL_NAME=`readlink -f $TOOL_NAME`

popd > /dev/null

mkdir -p third_party

pushd third_party > /dev/null

mv $TOOL_NAME .
TOOL_NAME=`basename $TOOL_NAME`
echo "$TOOL_NAME/target" >> .gitignore
echo "$TOOL_NAME/in" >> .gitignore

popd > /dev/null
rm -rf third_party_tmp

echo "Suppose that it's traditional AtCoder tools and try to build" # but we will not build :(

pushd third_party/$TOOL_NAME > /dev/null

if [[ -d third_party/$TOOL_NAME/in/ ]]; then
  echo "in/ does not exist. abort"
  exit 2
fi

cat << EOL > ../../bin/generator_multi.sh
#!/bin/bash

set -eu
cd \`dirname \$0\`/..
ls third_party/$TOOL_NAME/in/*
EOL

cargo build --release

popd > /dev/null

script/generate_testcases.sh
# TODO: setup stdin directory

echo "complete!"
