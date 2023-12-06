#lang racket 

(require test-engine/racket-tests)

;; once again I chose to use megaparsack
;; to do the parsing.
(require 
  megaparsack
  megaparsack/text
  data/monad
  data/applicative)

(define INPUT-FILE-PATH "../inputs/day5.example")


(struct Mapping-Range (from to len) #:transparent)

;; ranges is a listof Mapping-Range
(struct Mapping (ranges) #:transparent)

;; definition of a parser that matches a Mapping-Range
(define mapping-range/p
  (do
    [to <- integer/p]
    (many+/p space/p)
    [from <- integer/p]
    (many+/p space/p)
    [len <- integer/p]
    (or/p eof/p (string/p "\n"))
    (pure (Mapping-Range from to len))))

;; definition of a parser that matches a Mapping
(define mapping/p
  (do
    (many-until/p any-char/p #:end (string/p "\n"))
    [ranges <- (many+/p mapping-range/p)]
    (pure ranges)))

;; definition of a parser that parses the input
(define input/p
  (do
    (string/p "seeds: ")
    [seeds <- (many+/p integer/p #:sep (char/p #\ ))]
    (string/p "\n\n")
    [mappings <- (many+/p mapping/p #:sep (string/p "\n"))]
    (pure (list seeds mappings))))         


(define (parse-in input)
  (parse-result! (parse-string input/p input)))


(define (in-range? start range-length number)
  (and (>= number start) (< number (+ start range-length))))

(check-expect (in-range? 20 2 20) #t)
(check-expect (in-range? 20 2 21) #t)
(check-expect (in-range? 20 2 22) #f)
(check-expect (in-range? 20 2 19) #f)


;; Mapping-Range Number -> Number

;; given a mapping range and a number, returns
;; to what other number the given number is mapped to,
;; or returns false if the given number isn't in the
;; mapping-range interval.
(define (map-number-one-range mapping-range number) 
  (cond 
    [(in-range? (Mapping-Range-from mapping-range) (Mapping-Range-len mapping-range) number)
     (+ (Mapping-Range-to mapping-range) (- number (Mapping-Range-from mapping-range)))] ;mapped number = ( to + (number-from) )
    [else #f]))

(check-expect (map-number-one-range (Mapping-Range 50 98 2) 51) 99)
(check-expect (map-number-one-range (Mapping-Range 50 98 2) 60) #f)
(check-expect (map-number-one-range (Mapping-Range 50 98 2) 98) #f)

;; Mapping Integer -> Integer

;; given a mapping range and a number, returns to what
;; other number the given number is mapped to.
(define (map-number mapping number)
  (cond 
    [(empty? mapping) number]
    [(map-number-one-range (first mapping) number) (map-number-one-range (first mapping) number)]
    [else (map-number (rest mapping) number)]))

;; (listof Mapping) Integer -> Integer

;; given a number that represents a property of
;; the plantation (seed, soil, fertilizer, etc)
;; and a list of mappings, such that the first element of
;; the given list refers to the mapping of the property,
;; returns to what property the given number is matched to.
(define (property->location mappings property)
  
  (cond
    [(empty? mappings) property] ; reached location
    [else (property->location (rest mappings) (map-number (first mappings) property))]))



(define (part1 input)
  (define (solve-p1 seeds mappings)
  
  (apply min
    (map (lambda (s) (property->location mappings s))
         seeds))) ;; map each seed to location

  (let ([parsed-input (parse-in (file->string input))]) 
      (solve-p1 (first parsed-input) (second parsed-input))))

;; Integer (listof Integer) -> Bool
;; determine if a given number is in any
;; of the seed ranges (for part 2)
(define (in-some-seed-range? number seeds)
  (cond
    [(empty? seeds) #f]
    [(in-range? (first seeds) (second seeds) number)]
    [else (in-some-seed-range? number (rest (rest seeds)))]))

;; Integer (listof Integer) (listof Mapping) -> Bool

;; determines if a given number can be reached by
;; starting at some seed and ending at the number by following a
;; sequence of mappings. This is done using a reversed mapping
;; and going from the number back to the seed.
(define (has-reverse-mapping? number seeds mappings)
  (cond
    [(empty? mappings) (in-some-seed-range? number seeds)]
    [else (has-reverse-mapping? (map-number (first mappings) number) seeds (rest mappings))]))

;; Mapping-Range -> Mapping-Range
(define (reverse-mapping-range mapping-range)
  (Mapping-Range (Mapping-Range-to mapping-range)
                 (Mapping-Range-from mapping-range)
                 (Mapping-Range-len mapping-range)))

;; Mapping -> Mapping
(define (reverse-mapping mapping)
  (map reverse-mapping-range mapping))

;; listof Mapping -> listof Mapping

;; returns a list of mappings in such a way
;; that if a number x was mapped to y, now
;; y is mapped to x.
(define (reverse-mappings mappings)
  (map reverse-mapping (reverse mappings)))  


;; the first thought is to check for all numbers in the seeds
;; range and find to what location it maps to, then get the min,
;; this is too slow, since we have to check every number in the interval,
;; which amounts to more than 10¹⁰ checks.

;; the first optimization is then to do the reverse path: start from the
;; location and reverse map all the way back to the seeds, if such path
;; exists, then the first index found is the answer.

;; the second is based on the fact the range of all lenghts
;; in the inputs are very large (> 10⁵), therefore, if there is not
;; a path from location 0 to a seed, we don't have to check for 1, 2, 3, etc,
;; we can check 10⁵, 2*10⁵, 3*10⁵, and so on..., this makes the computation
;; really fast.
(define (part2 input)
  (define MIN-STEP 100000)
  
  (let ([parsed-input (parse-in (file->string input))])
    (let ([reversed-mappings (reverse-mappings (second parsed-input))])

      (define (get-precise idx)
        (cond
          [(not (has-reverse-mapping? idx (first parsed-input) reversed-mappings)) (add1 idx)]
          [(get-precise (sub1 idx))]))
                           
      (define (solve-p2 idx)
          (cond
            [(has-reverse-mapping? idx (first parsed-input) reversed-mappings)
             (get-precise idx)]
            [else (solve-p2 (+ idx MIN-STEP))]))
      
      (solve-p2 0))))
  
(test)


(part1 "../inputs/day5.input")
(part2 "../inputs/day5.input")

