define showvec
  if $argc < 1
    printf "usage: showvec <char**> [len|-1]\n"
  else
    set $v = $arg0
    set $n = ($argc >= 2) ? $arg1 : -1
    set $i = 0
    if $n == -1
      while $v[$i]
        printf "[%d] %s\n", $i, $v[$i]
        set $i = $i + 1
      end
    else
      while $i < $n
        printf "[%d] %s\n", $i, $v[$i]
        set $i = $i + 1
      end
    end
  end
end