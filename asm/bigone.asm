        .org $0600

        LDX #$FF
        TXS
        LDA #$10
        LDY #$20

        ADC #$05
        SBC #$03

        AND #$0F
        ORA #$F0
        EOR #$0F
        BIT $00

        ASL
        LSR
        ROL
        ROR

        PHA
        PHP
        PLA
        PLP
        TSX
        TXS

        STA $2000
        STX $2001
        STY $2002

        JSR Subroutine
Subroutine:
        LDA #$99
        RTS

Loop:
        JMP Loop
        .word Loop
