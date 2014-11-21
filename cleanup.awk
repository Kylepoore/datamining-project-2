#!/bin/awk -f

BEGIN{
  inreview = 0;  
  count = 0;
  skip = 0;
  
}


/^-----.*BEGIN/ {
  inreview = 1;
  print "-------------------------"
  next;
}

/^-----.*END/ {
  inreview = 0;
  count = 0;
  next;
}

inreview == 1 && count == 0{
  print $0;
  count++;
  next;
}

/^img width/{
  #print "skipping";
  skip = 1;
  next;
}

/^p$/ && skip == 1{
  #print "not skipping";
  skip = 0;
  next; 
}

inreview == 1 && skip == 0 {
  print $0;
}
