#!/bin/bash
base_url='http://www.imdb.com'

function search {
  title="`echo $1 | sed -e 's/ /%20/g'`"
  curl -m30 "$base_url/find?ref_=nv_sr_fn&q=$title&s=tt" 2> /dev/null
}

function unescape {
  echo "$1" | sed -e 's/&#x22;/"/g' -e 's/&#x27;/'"'"'/g' -e 's/&.{4};//g'


}

function untag {
  echo "$1" | egrep -vw '^[/]?\(small|div|href|br|b|hr\)'
}

IFS=$'\n'

for title in $(cat movies.list)
#for title in avengers 
  do
    sleep 2
    echo "title: $title"
    movie_page=`search "$title" | tr '<>' '\n\n' | egrep '^a href="/title' | cut -d'"' -f2 |head -1`
    echo "movie page: $movie_page"
    review_page=`curl -m30 $base_url$movie_page 2> /dev/null | tr '<>' '\n\n' | egrep '^a href="/title.*/reviews\?.*urv' | cut -d'"' -f2`
    echo "review page: $review_page"
    result_url="$base_url$review_page&filter=chrono"
    echo "result url: $result_url"
    num_reviews=`curl -m30 "$result_url" 2> /dev/null | tr '<>' '\n\n' | grep "reviews in total" | awk '{print $1}'`
    echo "number of reviews: $num_reviews"
    if [[ ! $num_reviews = "" ]]
      then
      for i in $(seq 0 10 $num_reviews)
        do
          review_result=`curl -m30 "$result_url&start=$i" 2> /dev/null | tr '<>' '\n\n' | egrep '[^\s]+' | sed 's/\/hr/------------------/g'`
          #echo -e "$review_result"
          on=0
          in_rev=0
          for line in `echo "$review_result" | uniq`
            do
              l=`echo $line | egrep -o 'showtimes/[0-9]{2,3}\.gif|Was the above review useful'`
              if [[ $l = "" ]]
                then
                :
              elif [[ $l = "Was the above review useful" ]]
                then
                in_rev="0"
                echo "-----REVIEW END-----"
              else
                in_rev="1"
                echo "-----REVIEW BEGIN-----"
                echo "`echo "$l" | grep -o '[0-9]*'`/100"
              fi
              if [[ $on = "1" && $in_rev = "1" ]]
                then
                if echo $line | egrep -q '^/p'
                  then
                  on="0"
                  continue
                fi
                echo $(untag "`unescape "$line"`")
              elif echo $line | egrep -q '^p\s*$'
                then
                on="1"
              fi
            done
        done
    fi
  done

