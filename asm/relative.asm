.org $0600
  LDA #$01
  CMP #$02
  BNE notequal
  STA $22
notequal:
  BRK