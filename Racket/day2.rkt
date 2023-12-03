#lang racket 

(require test-engine/racket-tests)

;; I chose to use the library megaparsack by Alexis King
;; (https://docs.racket-lang.org/megaparsack/index.html) to do
;; the parsing.
(require 
  megaparsack
  megaparsack/text
  data/monad
  data/applicative)

(define INPUT-FILE-PATH "../inputs/day2.txt")
(define input-string-list (file->lines INPUT-FILE-PATH))


(define MAX-RED 12)
(define MAX-GREEN 13)
(define MAX-BLUE 14)

(struct Sack (red green blue) #:transparent)
(struct Game (id sacks) #:transparent)

; ===== BEGIN PARSING ======
;; given a list of pairs representing an item on a sack, returns
;; a sack, check the examples.

(define (parsed-sack->sack lst)
  (define (find-color lst color)
    (cond
      [(empty? lst) 0]
      [(equal? (first (first lst)) color) (second (first lst))]
      [else (find-color (rest lst) color)]))
  
  (Sack (find-color lst "red")
        (find-color lst "green")
        (find-color lst "blue")))

;; definition of a parser that matches
;; something of the form "15  blue", "4 red", etc.
(define one-item/p
  (do
    [value <- integer/p]
    (many/p space/p)
    [color <- (or/p
     (string/p "red")
     (string/p "green")
     (string/p "blue"))]
    (pure (list color value))))

;; definition of a parser that matches
;; something of the form "1 blue, 2 green", "1 green, 3 red, 6 blue", etc.
(define sack/p
  (do
    [sack <- (many+/p one-item/p #:sep (string/p ", "))]
    (pure (parsed-sack->sack sack))))

;; definition of a parser that matches a game
(define game/p
  (do
    (string/p "Game")
    (many/p space/p)
    [id <- integer/p]
    (char/p #\:)
    (many/p space/p)
    [sacks <- (many/p sack/p #:min 1 #:sep (string/p "; "))]
    (pure (Game id sacks))))


;; String -> Game
;; parses a game, given as a String to a game structure
(define (parse-game line)
  (parse-result! (parse-string game/p line)))

(check-expect
  (parse-game "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue")
  (Game 2 (list (Sack 0 2 1) (Sack 1 3 4) (Sack 0 1 1))))
(check-expect
  (parse-game "Game 18: 10 red, 11 green; 132 green, 8 blue, 19 red")    ;multi-digits
  (Game 18 (list (Sack 10 11 0) (Sack 19 132 8))))
(check-expect
  (parse-game "Game 231: 10 red; 132 green; 32 blue")    ;weird
  (Game 231 (list (Sack 10 0 0) (Sack 0 132 0) (Sack 0 0 32))))


;; games is a list of Game
(define games (map parse-game input-string-list))

; ===== END PARSING ======


;; Game -> Bool
;; returns wether a given game is valid or not,
;; a game is valid iff all of its sacks dont contain more than 
;; `MAX-RED` red elements, `MAX-GREEN` green elements and `MAX-BLUE` blue elements.
(define (valid-game? game)
  (andmap (lambda (sack) 
            (and 
              [<= (Sack-red sack) MAX-RED]
              [<= (Sack-green sack) MAX-GREEN]
              [<= (Sack-blue sack) MAX-BLUE]))
          (Game-sacks game)))


(check-expect (valid-game? (Game 1 (list (Sack 12 13 14)) )) #t)
(check-expect (valid-game? (Game 2 (list (Sack 13 13 13) ))) #f) ; red fails
(check-expect (valid-game? (Game 3 (list (Sack 11 14 13) ))) #f) ; green fails
(check-expect (valid-game? (Game 4 (list (Sack 11 7 19) ))) #f)  ; blue fails
(check-expect (valid-game? (Game 5 (list (Sack 20 20 20) ))) #f)  ; all fail
(check-expect (valid-game? (Game 1 (list (Sack 1 1 1) (Sack 15 1 1)) )) #f)  ; second sack fails
(check-expect (valid-game? (Game 7 (list (Sack 2 3 9) (Sack 1 2 3) (Sack 7 4 14) (Sack 12 13 14)))) #t)  ; ok


;; gets the power of a game.
(define (game-power game)
  (apply * (list 
    [apply max (map (lambda (sack) (Sack-red sack)) (Game-sacks game))]
    [apply max (map (lambda (sack) (Sack-green sack)) (Game-sacks game))]
    [apply max (map (lambda (sack) (Sack-blue sack)) (Game-sacks game))])))


(check-expect (game-power (Game 7 (list (Sack 2 3 9) (Sack 1 2 3) (Sack 7 4 14) (Sack 12 13 14)))) (* 12 13 14))  
(check-expect (game-power (Game 7 (list (Sack 2 5 9) (Sack 1 2 14) (Sack 7 4 11)))) 
              (* (max 2 1 7) (max 5 2 4) (max 9 14 11)))  

;; part one
(define (part1 games)
  (apply + (map
             (lambda (game) (Game-id game))
             (filter valid-game? games))))

(part1 games)

;; part two
(define (part2 games)
  (apply + (map
             game-power
             games)))

(part2 games)

(test)

