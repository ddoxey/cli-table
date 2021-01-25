# cli-table
Formatted tables on the CLI

# Build

./build.sh

# Test

./build/bin/test_table

# Install

cd build && sudo make install

# USAGE:

Given some file data.csv, such that:
```
: cat data.csv
a,b,c,d,e
f,g,h,i,j
k,l,m,n,o
p,r,s,t,u
v,w,x,y,z

: table data.csv
┌───┬───┬───┬───┬───┐
│ a │ b │ c │ d │ e │
│ f │ g │ h │ i │ j │
│ k │ l │ m │ n │ o │
│ p │ r │ s │ t │ u │
│ v │ w │ x │ y │ z │
└───┴───┴───┴───┴───┘

: cat data.csv | table
┌───┬───┬───┬───┬───┐
│ a │ b │ c │ d │ e │
│ f │ g │ h │ i │ j │
│ k │ l │ m │ n │ o │
│ p │ r │ s │ t │ u │
│ v │ w │ x │ y │ z │
└───┴───┴───┴───┴───┘
```
```
: cat datb.csv
a
b,c
d,e,f
g,h,i
j,k

l,m,n,o
p,r,v,w
x,y
z

: table datb.csv
┌───────────────┐
│ a             │
├───────┬───────┤
│ b     │ c     │
├─────┬─┴──┬────┤
│ d   │ e  │ f  │
│ g   │ h  │ i  │
├─────┴─┬──┴────┤
│ j     │ k     │
├───────┴───────┤
│               │
├───┬───┬───┬───┤
│ l │ m │ n │ o │
│ p │ r │ v │ w │
├───┴───┼───┴───┤
│ x     │ y     │
├───────┴───────┤
│ z             │
└───────────────┘
```

# Stylizing with SGR

You can stylize the table by declaring conditions for applying
alignment and SGR parameters.

```
echo 'hello,world' | table -s '{"col":[{"where":{"n":0},"sgr":[1,32]},{"where":{"n":1},"sgr":[1,31]}]}'
```

Or in a case where your styling rules are in a JSON file.

```
echo 'hello,world' | table -s style.json
```

See the scripts in the *demo* folder for more examples.

```
bash demo/colors.sh
...

bash demo/old_glory.sh
...

bash demo/spreadsheet.sh
...
```
