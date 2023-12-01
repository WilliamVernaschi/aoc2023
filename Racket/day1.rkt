#lang racket 

(require test-engine/racket-tests)

(define INPUT-FILE-PATH "../inputs/day1.txt")
(define input-string-list (file->lines INPUT-FILE-PATH))

(define lookup (hash 
    "one" "1"
    "two" "2"
    "three" "3"
    "four" "4"
    "five" "5"
    "six" "6"
    "seven" "7"
    "eight" "8"
    "nine" "9"
    "1" "1"
    "2" "2"
    "3" "3"
    "4" "4"
    "5" "5"
    "6" "6"
    "7" "7"
    "8" "8"
    "9" "9"))

;; String -> list of Naturals

;; given a string, returns a list of all the values
;; of all substrings in order
(define (get-all-matchings str)
  (for/list ([(key value) (in-hash lookup)]
             #:when (string-contains? str key)
             )
    value))

(check-expect (get-all-matchings "fourthreefour55one")    (list "4" "5" "5"))
(check-expect (get-all-matchings "oneight")   (list "1" "8"))
(check-expect (get-all-matchings "oneeight")  (list "1" "8"))
(check-expect (get-all-matchings "93")        (list "9" "3"))
(check-expect (get-all-matchings "five")       (list "5"))
(check-expect (get-all-matchings "ontwthrefoour1")  (list "1"))
  

;; String -> Natural

;; gets the two digit number formed by the concatenation of the
;;   **values** of the first and last occurrance of any string
;;   that is in `lookup`. check the examples.
(define (string->two-digit-number str)
  (define matchings (get-all-matchings str))

  (string->number (string-append (first matchings) (last matchings))))



(check-expect (string->two-digit-number "four55")    45)
(check-expect (string->two-digit-number "oneight")   18)
(check-expect (string->two-digit-number "oneeight")  18)
(check-expect (string->two-digit-number "93")  93)
(check-expect (string->two-digit-number "five")  55)
(check-expect (string->two-digit-number "ontwthrefoour1")  11)

(test)


(apply + (map string->two-digit-number input-string-list))


