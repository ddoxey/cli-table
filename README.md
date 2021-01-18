# cli-table
Formatted tables on the <span style="color:#ff0000">CLI</span>

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
│       a       │
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
│       z       │
└───────────────┘
```

