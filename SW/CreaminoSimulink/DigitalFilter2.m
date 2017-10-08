function DigitalFilter

    
    NF = zeros (1,7);
    NF_Notch = zeros (1,7);
    
    DF = zeros (1,7);   
    DF_Notch = zeros (1,7);
    
    INF = zeros(7,CHnum);
    OUTF = zeros(7,CHnum);
    INF_Notch = zeros(7,CHnum);
    OUTF_Notch = zeros(7,CHnum);

    
    function [OUTF_notch] = IIR6Notch(cf, Word)

        for cf2 = 7: -1: 1
            INF_Notch(cf2)(cf) = INF_Notch(cf2 - 1)(cf);
            OUTF_Notch(cf2)(cf) = OUTF_Notch(cf2 - 1)(cf);
        end
        
        INF_Notch(1)(cf) = Word;
        OUTF_Notch(1)(cf) = NF_Notch(1) * INF_Notch(1)(cf) + NF_Notch(2) * INF_Notch(2)(cf) + NF_Notch(3) * INF_Notch(3)(cf) + NF_Notch(4) * INF_Notch(4)(cf) + NF_Notch(5) * INF_Notch(5)(cf) + NF_Notch(6) * INF_Notch(6)(cf) + NF_Notch(7) * INF_Notch(7)(cf);
        OUTF_Notch(1)(cf) = OUTF_Notch(1)(cf) - DF_Notch(2) * OUTF_Notch(2)(cf) - DF_Notch(3) * OUTF_Notch(3)(cf) - DF_Notch(4) * OUTF_Notch(4)(cf) - DF_Notch(5) * OUTF_Notch(5)(cf) - DF_Notch(6) * OUTF_Notch(6)(cf) - DF_Notch(7) * OUTF_Notch(7)(cf);                
    end
    
    
    function [OUTF] = IIR6(cf, Word)
    
        for cf2 = 7: -1: 1
            INF(cf2)(cf) = INF(cf2 - 1)(cf);
            OUTF(cf2)(cf) = OUTF(cf2 - 1)(cf);
        end
        INF(1)(cf) = Word;
        OUTF(1)(cf) = NF(1) * INF(1)(cf) + NF(2) * INF(2)(cf) + NF(3) * INF(3)(cf) + NF(4) * INF(4)(cf) + NF(5) * INF(5)(cf) + NF(6) * INF(6)(cf) + NF(7) * INF(7)(cf);
        OUTF(1)(cf) = OUTF(1)(cf) - DF(2) * OUTF(2)(cf) - DF(3) * OUTF(3)(cf) - DF(4) * OUTF(4)(cf) - DF(5) * OUTF(5)(cf) - DF(6) * OUTF(6)(cf) - DF(7) * OUTF(7)(cf);                
    end
    

    function [IIR6] = FilteredWord(cf, Word)
        IIR6(cf, Word);
    end
    
    function [IIR6Notch] = NotchFilteredWord(cf, Word)
        IIR6Notch(cf, Word);
    end
    

    function NoFilter
      
        NF(1) = 1;
        NF(2) = 0;  
        NF(3) = 0;
        NF(4) = 0;
        NF(5) = 0;       
        NF(6) = 0;
        NF(7) = 0;
        
        DF(1) =  1;
        DF(2) =  0;
        DF(3) =  0;
        DF(4) =  0;
        DF(5) =  0;
        DF(6) =  0;
        DF(7) =  0;
        
    end
    
    function IIR_05_65_250SPS
         
        NF(1) =  0.179516478347;
        NF(2) =  0;  
        NF(3) = -0.5385494350409;
        NF(4) =  0;
        NF(5) = 0.5385494350409;       
        NF(6) = 0;
        NF(7) = -0.179516478347;
        
        DF(1) =  1;
        DF(2) =  -2.869187222081;
        DF(3) =  2.975880684331;
        DF(4) =  -1.666797891076;
        DF(5) =  0.8747739395112;
        DF(6) =  -0.3062731476477;
        DF(7) =  -0.00839346201296;
        
    end
    
    
     
      function SetNotch(byte SampleRate) 
        
        switch SampleRate  
            case 0x06 
		SetNotch_125SPS ;
            case 0x05 
		SetNotch_250SPS ;
            case 0x04  
		SetNotch_500SPS ;
            case 0x03  
		SetNotch_1KSPS ;
            case 0x02  
		SetNotch_2KSPS ;
            case 0x01  
		SetNotch_4KSPS ;
            case 0x00  
		SetNotch_8KSPS ;
        end
    end
    
    
    function SetFilter(n, SampleRate) 
    
        switch n  
            case 0   
		NoFilter ;
            case 1   
                switch SampleRate  
                    case 0x06   
			IIR_05_65_125SPS ;
                    case 0x05   
			IIR_05_65_250SPS ;
                    case 0x04   
			IIR_05_65_500SPS ;
                    case 0x03   
			IIR_05_65_1KSPS ;
                    case 0x02   
			IIR_05_65_2KSPS ;
                    case 0x01   
			IIR_05_65_4KSPS ;
                    case 0x00   
			IIR_05_65_8KSPS ;
                end  
            case 2   
		switch SampleRate  
                    case 0x06   
			IIR_05_40_125SPS ;
                    case 0x05   
			IIR_05_40_250SPS ;
                    case 0x04   
			IIR_05_40_500SPS ;
                    case 0x03   
			IIR_05_40_1KSPS ;
                    case 0x02   
			IIR_05_40_2KSPS ;
                    case 0x01   
			IIR_05_40_4KSPS ;
                    case 0x00   
			IIR_05_40_8KSPS ;
                end  
            case 3  
		 switch SampleRate  
                    case 0x06   
			IIR_05_20_125SPS ;
                    case 0x05   
			IIR_05_20_250SPS ;
                    case 0x04   
			IIR_05_20_500SPS ;
                    case 0x03   
			IIR_05_20_1KSPS ;
                    case 0x02   
			IIR_05_20_2KSPS ;
                    case 0x01   
			IIR_05_20_4KSPS ;
                    case 0x00   
			IIR_05_20_8KSPS ;
                end  
            case 4   
		switch SampleRate  
                    case 0x06   
			IIR_2_65_125SPS ;
                    case 0x05   
			IIR_2_65_250SPS ;
                    case 0x04   
			IIR_2_65_500SPS ;
                    case 0x03   
			IIR_2_65_1KSPS ;
                    case 0x02   
			IIR_2_65_2KSPS ;
                    case 0x01   
			IIR_2_65_4KSPS ;
                    case 0x00   
			IIR_2_65_8KSPS ;
                end  
            case 5   
		switch SampleRate  
                    case 0x06   
			IIR_2_40_125SPS ;
                    case 0x05   
			IIR_2_40_250SPS ;
                    case 0x04   
			IIR_2_40_500SPS ;
                    case 0x03   
			IIR_2_40_1KSPS ;
                    case 0x02   
			IIR_2_40_2KSPS ;
                    case 0x01   
			IIR_2_40_4KSPS ;
                    case 0x00   
			IIR_2_40_8KSPS ;
                end  
            case 6  
		 switch SampleRate  
                    case 0x06   
			IIR_2_20_125SPS ;
                    case 0x05   
			IIR_2_20_250SPS ;
                    case 0x04   
			IIR_2_20_500SPS ;
                    case 0x03   
			IIR_2_20_1KSPS ;
                    case 0x02   
			IIR_2_20_2KSPS ;
                    case 0x01   
			IIR_2_20_4KSPS ;
                    case 0x00   
			IIR_2_20_8KSPS ;
                       
                end  
                    
        end
        
    end
   
