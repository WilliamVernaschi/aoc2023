#lang racket 

(require test-engine/racket-tests)

;; once again I chose to use megaparsack
;; to do the parsing.
(require 
  megaparsack
  megaparsack/text
  data/monad
  data/applicative)

(define INPUT-FILE-PATH "../inputs/day4.txt")
(define (input->lines input)
  (file->lines input))

(struct Game (id my-cards winning-cards) #:transparent)


; ===== BEGIN PARSING ======

;; parser for a number followed by any number of 
;; spaces (possibly none), only captures the number
(define number-space/p
  (do
    [x <- integer/p]
    (many/p space/p)
    (pure x)))

;; definition of a parser that matches a card
(define card/p
  (do
    (string/p "Card")
    (many/p space/p)
    [id <- integer/p]
    (char/p #\:)
    (many/p space/p)
    [winning-cards <- (many/p number-space/p)]
    (char/p #\|)
    (many/p space/p)
    [my-cards <- (many/p number-space/p)]

    (pure (Game id my-cards (list->set winning-cards)))))



;; String -> Game
;; parses a game, given as a String to a game structure
(define (parse-card line)
  (parse-result! (parse-string card/p line)))

(check-expect (parse-card "Card 121: 3 4 123 43 | 1 3 123 14") 
              (Game 121 (list 1 3 123 14) (set 3 4 123 43) ))
(check-expect (parse-card "Card 5: 1 | 2") 
              (Game 5 (list 2) (set 1)))

(test)
; ===== END PARSING ======

;; Game -> Integer
;; given a game, counts the number of cards that intersect.
;; in O(nlogm), where m is the number of winning cards
;; and n is my number of cards.
(define (count-winning game)
  (cond 
    [(empty? (Game-my-cards game)) 0]
    [(set-member? (Game-winning-cards game) (first (Game-my-cards game)))
     (add1 (count-winning
             (Game (Game-id game) (rest (Game-my-cards game)) (Game-winning-cards game))))]
    [else  
      (count-winning
             (Game (Game-id game) (rest (Game-my-cards game)) (Game-winning-cards game)))]))

(check-expect (count-winning (Game 3 (list 1 2 3) (set 1 2 3)) )
              3)

(check-expect (count-winning (parse-card "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19")) 
              2)

(test)
  

;; returns x**y, however, if y is not positive, returns 0
(define (expt-integer x y)
  (if (< y 0)
    0
    (expt x y)))

(define (part1 input)
  (apply + 
    (map (lambda (parsed-card) (expt-integer 2 (sub1 (count-winning parsed-card))))
        (map parse-card (input->lines input)))))


;; solves part 2 in O(n* O(count-winning))
;; there is a fancy name for this technique, but i can't recall it
(define (part2 input)
  
  (define (make-initial-vector n)
  (begin
    (define vec (make-vector (+ n 1)))
    (vector-set! vec 0 1)
    vec))
  
  (define (iterate vec idx cards pfx-sum)
    (cond
      [(equal? idx (sub1 (vector-length vec))) 0]
      [else
        (let ([num-winning (count-winning (first cards))])
          (begin
            (vector-set! vec (+ idx 1) (+ pfx-sum (vector-ref vec (+ idx 1)) ))
            (vector-set! vec (+ idx num-winning 1) (- (vector-ref vec (+ idx num-winning 1)) pfx-sum))
            
            (+ pfx-sum (iterate vec (+ idx 1) (rest cards) (+ pfx-sum (vector-ref vec (+ idx 1)) )))))]))

  (let ([cards-list (map parse-card (input->lines input))])
    (iterate (make-initial-vector (length cards-list)) 0 cards-list 1)))

(part1 INPUT-FILE-PATH)
(part2 INPUT-FILE-PATH)




