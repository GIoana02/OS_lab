
    gcc -Wall "$1" 2>&1 | grep -Eo "([0-9]+ warning\(s\)|[0-9]+ error\(s\))" | tr '\n' ' '
    
