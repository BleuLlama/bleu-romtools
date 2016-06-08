;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	ram.asm
;
;	what is where in RAM
;
;	2004,2006  Scott "Jerry" Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module ramlist

	; ram usage
	RAMLIST 	== rambase

	    ; timer
	timer		== RAMLIST+0	; w timer incremented in interrupt

	    ; random
	randval		== RAMLIST+2	; w random helper

	    ; 'at' variables
	atControl	== RAMLIST+4	; b internal control byte

	atFunc		== RAMLIST+5	; w address of function
	atTime		== RAMLIST+7	; w time to call the above

	at2Func		== RAMLIST+9	; w address of second function
	at2Time		== RAMLIST+11	; w time to call the above

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; input management

	INPMGMT		== RAMLIST+13

	    ; for the input routines...
	lastcoin	== INPMGMT+0	; b last coin port (for debouncing)
	thiscoin	== INPMGMT+1	; b this coin port

	laststart	== INPMGMT+2	; b last start port (for debouncing)
	thisstart	== INPMGMT+3	; b this start port

	thisp1p		== INPMGMT+4	; b this player 1 input
	lastp1p		== INPMGMT+5	; b last player 1 input

	thisp2p		== INPMGMT+6	; b this player 2 input
	lastp2p		== INPMGMT+7	; b last player 2 input

	thistest	== INPMGMT+8	; this test switch mode 
	lasttest	== INPMGMT+9	; last test switch mode

	    ; input counters
	INPCNTRS	== INPMGMT+10

	Cp1up		== INPCNTRS+0
	Cp1dn		== INPCNTRS+1
	Cp1lt		== INPCNTRS+2
	Cp1rt		== INPCNTRS+3

	Cp2up		== INPCNTRS+4
	Cp2dn		== INPCNTRS+5
	Cp2lt		== INPCNTRS+6
	Cp2rt		== INPCNTRS+7

	Cstart1		== INPCNTRS+8
	Cstart2		== INPCNTRS+9
	Ccoin1		== INPCNTRS+10
	Ccoin2		== INPCNTRS+11
	Ccoin3		== INPCNTRS+12


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; task system stuff

	TASKPOS		== INPCNTRS+13

	    ; for the task list system...
	taskid		== TASKPOS+0	; b task list index
	taskptr		== TASKPOS+1	; w points to the currrent task header

	    ; for tasks
	runMode		== TASKPOS+3	; b current mode
		M_TEST      == 0xFF	;   test switch is "on"	
		M_ATT	    == 0x00	;   insert coins/press start attract
		M_GAMEPLAY  == 0x01	;   task is running
		M_DEATH     == 0x02	;   death sequence
		M_BUYIN     == 0x03	;   Insert coins to continue
		M_SSAVER    == 0x04	;   Screen Saver is active

	runMode1	== TASKPOS+4	; b submode 1 (defined by task)
	runMode2	== TASKPOS+5	; b submode 2 (defined by task)

	credits		== TASKPOS+6	; w number of credits

	variant		== TASKPOS+8	; b selected variant (0 if none)
	nplayers	== TASKPOS+9	; b which start button was pressed?

	dipBases	== TASKPOS+10	; b what does the dipswitch say?
	basesLeft	== TASKPOS+11	; b number of bases left

        difficulty	== TASKPOS+12	; b difficulty from dipswitch
	    diffNORM	== 0x00			;   normal difficulty
	    diffHARD	== 0x01			;   hard difficulty


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; 'pactracker' variables

		; song init settings
	PTBASE		== TASKPOS+13	; - base of pactracker variables
	ptFrame		== PTBASE+0	; w points to current frame
	ptFIndex	== PTBASE+2	; b index into current frame
	ptFLen		== PTBASE+3	; b length of the current frame
	ptCountdown	== PTBASE+4	; b number of ticks until next step
	ptFlags		== PTBASE+5	; b operational flags
	    PTF_STEP	== 1		;   pattern stepped
	    PTF_PSTEP	== 2		;   skip to next pattern next tick
	    PTF_LOOP	== 7		;   song should loop

		; song playback settings
	ptSpeed		== PTBASE+6	; b current speed setting
	ptSong		== PTBASE+7	; w points to top of pattern list

		; cache variables
	PTCACHE		== PTBASE+9
	ptT0Cache	== PTCACHE+0	; w current track 0 word
	ptT1Cache	== PTCACHE+2	; w current track 1 word
	ptT2Cache	== PTCACHE+4	; w current track 2 word
	ptT3Cache	== PTCACHE+6	; w current track 3 word

		; playback variables
	ptTRACK0	== PTCACHE+8
	ptT0SVol	== ptTRACK0+0	; b track 0 volume speed
	ptT0SFreq	== ptTRACK0+1	; b track 0 frequency speed
	ptT0SWave	== ptTRACK0+2	; b track 0 waveform speed

	ptTRACK1	== ptTRACK0+3
	ptT1SVol	== ptTRACK1+0	; b track 1 volume speed
	ptT1SFreq	== ptTRACK1+1	; b track 1 frequency speed
	ptT1SWave	== ptTRACK1+2	; b track 1 waveform speed

	ptTRACK2	== ptTRACK1+3
	ptT2SVol	== ptTRACK2+0	; b track 2 volume speed
	ptT2SFreq	== ptTRACK2+1	; b track 2 frequency speed
	ptT2SWave	== ptTRACK2+2	; b track 2 waveform speed

		; pattern frame looping
	ptLOOPS		== ptTRACK2+3
	ptT0Loop	== ptLOOPS+0	; b track 0 loop position
	ptT0Cntr	== ptLOOPS+1	; b track 0 counter (if 0, no loops)
	ptT1Loop	== ptLOOPS+2	; b track 1 loop position
	ptT1Cntr	== ptLOOPS+3	; b track 1 counter (if 0, no loops)
	ptT2Loop	== ptLOOPS+4	; b track 2 loop position
	ptT2Cntr	== ptLOOPS+5	; b track 2 counter (if 0, no loops)
	ptT3Loop	== ptLOOPS+6	; b track 3 loop position
	ptT3Cntr	== ptLOOPS+7	; b track 3 counter (if 0, no loops)


		; volume dropoff: (simple envelope)
	ptENV		== ptLOOPS + 8
	pt0vtim		== ptENV+0	; b volume timing byte 
	pt0vdst		== ptENV+1	; b volume destination
	pt1vtim		== ptENV+2	; b volume timing byte
	pt1vdst		== ptENV+3	; b volume destination
	pt2vtim		== ptENV+4	; b volume timing byte
	pt2vdst		== ptENV+5	; b volume destination


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; user/task space
	USERRAM		== ptENV+10	; where user apps/tasks can use  ram

