DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

echo "Scanning ${1:-sites/all/modules/custom}..."
git diff origin/master... > $DIR/diff.txt
./vendor/bin/phpcs --report=json --extensions=php,module,inc,js,css ${1:-sites/all/modules/custom} > $DIR/phpcs.json || true
./vendor/bin/diffFilter --phpcs $DIR/diff.txt $DIR/phpcs.json
