#lang racket

(require test-engine/racket-tests)

(require 
  megaparsack
  megaparsack/text
  data/monad
  data/applicative)

(struct Race (time dis))

;; (listof Integer) (listof Integer) -> (listof Race)
(define (times-distances->races times distances)
  (cond
    [(empty? times) empty]
    [else (cons (Race (first times) (first distances) ) (times-distances->races (rest times) (rest distances)))]))
  
(define input/p
  (do
    (string/p "Time:")
    (many+/p space/p)
    [times <- (many+/p integer/p #:sep (many+/p (char/p #\ )))]
    (string/p "\n")
    (string/p "Distance:")
    (many+/p space/p)
    [distances <- (many+/p integer/p #:sep (many+/p (char/p #\ )))]
    (pure (times-distances->races times distances))))

(define (parse-input file)
  (parse-result! (parse-string input/p (file->string file)))) 

(define (get-num-wins race)
  (let ([discriminant (sqrt (- (sqr (Race-time race)) (* 4 (Race-dis race))) )])
    (add1 (- (exact-ceiling (sub1 (/ (+ (Race-time race) discriminant) 2)))
             (exact-floor (add1 (/ (- (Race-time race) discriminant) 2)))))))

(check-expect (get-num-wins (Race 7 9)) 4)
(check-expect (get-num-wins (Race 15 40)) 8)
(check-expect (get-num-wins (Race 30 200)) 9)

(test)

(define (part1 file)
  (define (solve-part1 races)
    (cond 
      [(empty? races) 1]
      [else (* (get-num-wins (first races)) (solve-part1 (rest races)))]))

  (solve-part1 (parse-input file)))


(define part2 (get-num-wins (Race 48938595 296192812361391)))

(part1 "../inputs/day6.input")
part2 ;; i hard coded my input

