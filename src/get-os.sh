kernel=$(uname)

config="[%%define operating_system \"$kernel\"]"

echo $config > config.ml