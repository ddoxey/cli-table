# cli-table
Formatted tables on the CLI

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
