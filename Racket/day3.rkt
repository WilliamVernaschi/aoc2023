#lang racket

(require test-engine/racket-tests)

(define INPUT-FILE-PATH "../inputs/day3.input")

(define (file->grid input)
  (list->vector (map (lambda (string) (list->vector (string->list string))) (file->lines input))))

(define (num-rows grid)
  (vector-length grid))

(define (num-columns grid)
  (vector-length (vector-ref grid 0)))

(struct Position (row column) #:transparent)

(define (is-symbol? char) (and (not (char-numeric? char)) (not (equal? #\. char))) )
(check-expect (is-symbol? #\+) #t)
(check-expect (is-symbol? #\*) #t)
(check-expect (is-symbol? #\0) #f)
(check-expect (is-symbol? #\7) #f)

;; Position -> Bool
;; returns true if `position` is a valid position on the game grid.
(define (valid-position? grid position)
  (and
   (>= (Position-row position) 0)
   (>= (Position-column position) 0)
   (< (Position-row position) (num-rows grid))
   (< (Position-column position) (num-columns grid))))

(check-expect (valid-position? (list->vector (list (list->vector (list #\1 #\2 #\3))
                                                        (list->vector (list #\4 #\* #\6))
                                                        (list->vector (list #\7 #\8 #\9))))
                                    (Position 2 2))
              #t)
   

(define dx/dy (list (Position -1 -1) (Position -1 0) (Position -1 1)
                    (Position 0 -1)                  (Position 0 1)
                    (Position 1 -1) (Position 1 0) (Position 1 1)))

;; listof Integer -> Integer

;; given a list of numbers, returns 0 if the length
;; of the list is not 2, and the product of the two
;; numbers otherwise
(define (gear-value numbers)
  (if (equal? (length numbers) 2)
      (* (first numbers) (second numbers))
      0))

(check-expect (gear-value '(7 11)) (* 7 11))
(check-expect (gear-value '(12)) 0)
(check-expect (gear-value '(1 2 3)) 0)


;; Position -> Position

;; given two positions, returns their sum, check the examples.
(define (add-position p1 p2)
  (Position (+ (Position-row p1) (Position-row p2))
            (+ (Position-column p1) (Position-column p2))))

(check-expect (add-position (Position 3 4) (Position 1 2)) (Position (+ 3 1) (+ 4 2))) 

;; vector of vector of char, Position -> char

;; returns the char at a given position on a matrix.
(define (get-at-position matrix position) (vector-ref (vector-ref matrix (Position-row position)) (Position-column position)))

(check-expect (get-at-position (list->vector (list (list->vector (list #\1 #\2 #\3))
                                                   (list->vector (list #\4 #\5 #\6))
                                                   (list->vector (list #\7 #\8 #\9))))
                               (Position 1 1))
              #\5)

;; list of vector of vector of char, Position -> pairof Integer Position

;; given a grid and a position that contains a number,
;; returns a pair containing the number and it's starting position.
(define (get-number-at-position matrix position)
  (define (get-left-border line idx)
    (cond 
      [(not (char-numeric? (vector-ref line idx))) (add1 idx)]
      [(equal? idx 0) 0]
      [else (get-left-border line (sub1 idx)) ]))
  (define (get-right-border line idx)
    (cond
      [(equal? idx (vector-length line)) idx]
      [(not (char-numeric? (vector-ref line idx))) idx]
      [else (get-right-border line (add1 idx) )]))
  
  (define cur-line (vector-ref matrix (Position-row position)))
  (define cur-col  (Position-column position))

  (list (string->number
     (list->string
      (vector->list
       (vector-copy
        (vector-ref matrix (Position-row position))
        (get-left-border  cur-line cur-col)
        (get-right-border cur-line cur-col)))))
  (Position (Position-row position) (get-left-border cur-line cur-col))))

(check-expect (get-number-at-position
               (list->vector (list (list->vector (list #\1 #\2 #\3))
                             (list->vector (list #\4 #\* #\6))
                             (list->vector (list #\7 #\8 #\9))))
               (Position 1 0))
              (list 4 (Position 1 0)))

(check-expect (get-number-at-position
               (list->vector (list (list->vector (list #\1 #\2 #\3))
                             (list->vector (list #\4 #\* #\6))
                             (list->vector (list #\7 #\8 #\9))))
               (Position 0 2))
              (list 123 (Position 0 0)))

(check-expect (get-number-at-position
               (list->vector (list (list->vector (list #\1 #\2 #\3))
                             (list->vector (list #\4 #\* #\6))
                             (list->vector (list #\7 #\8 #\9))))
               (Position 2 0))
              (list 789 (Position 2 0)))

;; vector of vector of char -> listof listof Number and Position

;; given a vector of vector of chars and position, returns a list of
;; pairs, each pair represents a number adjacent to the given position
;; and the starting position of the integer.
(define (get-adjacent-numbers grid position)
  (remove-duplicates
  (for/list ([offset dx/dy]
             #:when (and 
                     (let ([neighbour (add-position offset position)]) 
                       (valid-position? grid neighbour)
                       (char-numeric? (get-at-position grid neighbour)))))
    (get-number-at-position grid (add-position offset position)))))

(check-expect (get-adjacent-numbers (list->vector (list (list->vector (list #\. #\2 #\3))
                                                        (list->vector (list #\4 #\+ #\6))
                                                        (list->vector (list #\7 #\8 #\9))))
                                    (Position 1 1))
              (list (list 23 (Position 0 1))
                    (list 4 (Position 1 0))
                    (list 6 (Position 1 2))
                    (list 789 (Position 2 0))))


;; solves part 1
(define (part1 grid)
  (apply +
         (map
          (lambda (n) (first n))
          (remove-duplicates
           (apply append
            (for*/list ([row-idx (in-range 0 (num-rows grid))]
                        [col-idx (in-range 0 (num-columns grid))]
                        #:when (is-symbol? (get-at-position grid (Position row-idx col-idx))))
              (get-adjacent-numbers grid (Position row-idx col-idx) )))))))

;; solves part 2
(define (part2 grid)
  (for*/sum ([row-idx (in-range 0 (num-rows grid))]
             [col-idx (in-range 0 (num-columns grid))]
             #:when (equal? (get-at-position grid (Position row-idx col-idx)) #\*))
    (gear-value
     (map (lambda (n) (first n))
     (remove-duplicates
      (get-adjacent-numbers grid (Position row-idx col-idx) ))))))

;(part1 (file->grid "../inputs/day3_example.txt"))

(define grid (file->grid INPUT-FILE-PATH))

(test)

(part1 grid)
(part2 grid)
