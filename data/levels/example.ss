;; Example map file for the game.

;; Width and height, the dimensions of the following bytevectors.
(define size 8)

;; The wall tilemap.
(define walling
  #vu8(
       1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 0 0 1 0 0 1
	 1 0 1 0 1 0 0 1
	 1 0 0 0 0 0 0 1
	 1 0 0 0 0 0 0 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
       ))

;; The floor tilemap.
(define flooring
  #vu8(
       1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
       ))

;; The ceiling tilemap.
(define ceiling
  #vu8(
       1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
	 1 1 1 1 1 1 1 1
       ))

;; The first element corresponds to 1 on the tilemap, the second to 2,
;; et cetera. 0 on tilemaps is ALWAYS empty space.
(define bitmaps #("data/bitmaps/terra.bmp"))

;; Player starting position.
(define player-x 1)
(define player-y 4)
;; You face the topmost wall at the beginning, `player-angle' merely
;; rotates from that origin point. This example rotates to the right,
;; if you want to rotate left of the origin point remove the negation
;; sign. Okay, it actually is some fucked up shit and I don't know how
;; it works. Just play around with it. Smaller numbers work better.
;; (define player-angle -8.5)
(define player-direction 0.0)
