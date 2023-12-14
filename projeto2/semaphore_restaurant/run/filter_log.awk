BEGIN {
     FS = " ";
     f=1
     FieldSize[f++]=3;
     FieldSize[f++]=2;
     FieldSize[f++]=2;
     for(i=0;i<ngroups;i++) {
        FieldSize[f++] = 3;
     }
     FieldSize[f++]=4;
     for(i=0;i<ngroups;i++) {
        FieldSize[f++] = 3;
     }
}

/.*/ {
    if(NF==ngroups*2+4) {
#        print  "NOTFILTE " $0
        for(i=1; i<=14; i++) {
            if(i<ngroups+4) {
               if($i==prev[i]) {
                 printf("%*s ",FieldSize[i],".")
               }
               else printf ("%*s ",FieldSize[i],$i) 
               prev[i] = $i
            }
            else {
               printf ("%*s ",FieldSize[i],$i) 
            }
        }
        printf("\n")
    }
    else print $0
}

END{
}
