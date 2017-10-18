/*
 * The MIT License
 *
 * Copyright 2017 Alma Mater Studiorum - Università di Bologna.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package Filter;

/**
 * @author Matteo Chiesi - University of Bologna
 */

/*Digital Filter */
public class DigitalFilter {

    
    private final double NF[] = new double[7];
    double NF_Notch[] = new double[7];
    
    private final double DF[] = new double[7];   
    double DF_Notch[] = new double[7];
    
    private final double[][] INF;
    private final double[][] OUTF;
    
    private final double[][] INF_Notch;
    private final double[][] OUTF_Notch;
    
    
    public DigitalFilter(int Channels){
        
        INF = new double[7][Channels];
        OUTF = new double[7][Channels];
        INF_Notch = new double[7][Channels];
        OUTF_Notch = new double[7][Channels];
    }
    
    /*6th order Notch Filter Formula*/
    private double IIR6Notch(int cf, double Word)
    {
        for (int cf2 = 6; cf2 > 0; cf2--)
        {
            INF_Notch[cf2][cf] = INF_Notch[cf2 - 1][cf];
            OUTF_Notch[cf2][cf] = OUTF_Notch[cf2 - 1][cf];
        }
        
        INF_Notch[0][cf] = Word;
        OUTF_Notch[0][cf] = NF_Notch[0] * INF_Notch[0][cf] + NF_Notch[1] * INF_Notch[1][cf] +
                            NF_Notch[2] * INF_Notch[2][cf] + NF_Notch[3] * INF_Notch[3][cf] + 
                            NF_Notch[4] * INF_Notch[4][cf] + NF_Notch[5] * INF_Notch[5][cf] + 
                            NF_Notch[6] * INF_Notch[6][cf];
        
        return OUTF_Notch[0][cf] = OUTF_Notch[0][cf] - 
                                   DF_Notch[1] * OUTF_Notch[1][cf] - 
                                   DF_Notch[2] * OUTF_Notch[2][cf] - 
                                   DF_Notch[3] * OUTF_Notch[3][cf] - 
                                   DF_Notch[4] * OUTF_Notch[4][cf] - 
                                   DF_Notch[5] * OUTF_Notch[5][cf] - 
                                   DF_Notch[6] * OUTF_Notch[6][cf];                
        }
    
    /*6th order Band Pass Filter Formula*/
    private double IIR6(int cf, double Word)
        {
                for (int cf2 = 6; cf2 > 0; cf2--)
                {
                    INF[cf2][cf] = INF[cf2 - 1][cf];
                    OUTF[cf2][cf] = OUTF[cf2 - 1][cf];
                }
                INF[0][cf] = Word;
                OUTF[0][cf] = NF[0] * INF[0][cf] + 
                              NF[1] * INF[1][cf] + 
                              NF[2] * INF[2][cf] + 
                              NF[3] * INF[3][cf] + 
                              NF[4] * INF[4][cf] + 
                              NF[5] * INF[5][cf] + 
                              NF[6] * INF[6][cf];
                
                return OUTF[0][cf] = OUTF[0][cf] - 
                                     DF[1] * OUTF[1][cf] - 
                                     DF[2] * OUTF[2][cf] - 
                                     DF[3] * OUTF[3][cf] - 
                                     DF[4] * OUTF[4][cf] - 
                                     DF[5] * OUTF[5][cf] - 
                                     DF[6] * OUTF[6][cf];                
        }
    

    public double FilteredWord(int cf, double Word){
        return IIR6(cf, Word);
    }
    
    public double NotchFilteredWord(int cf, double Word){
        return IIR6Notch(cf, Word);
    }
    
    /*Filter Coefficients*/
    private void NoFilter(){
      
        NF[0] = 1;
        NF[1] = 0;  
        NF[2] = 0;
        NF[3] = 0;
        NF[4] = 0;       
        NF[5] = 0;
        NF[6] = 0;
        
        DF[0] =  1;
        DF[1] =  0;
        DF[2] =  0;
        DF[3] =  0;
        DF[4] =  0;
        DF[5] =  0;
        DF[6] =  0;
        
    }
    
    private void IIR_05_65_125SPS(){
         
        NF[0] =  0.9509718879234;
        NF[1] =  0;  
        NF[2] = -2.85291566377;
        NF[3] =  0;
        NF[4] = 2.85291566377;       
        NF[5] = 0;
        NF[6] = -0.9509718879234;
        
        DF[0] =  1;
        DF[1] =  -1.451767017625e-15;
        DF[2] =  -2.899479594612;
        DF[3] =  2.609999245536e-15;
        DF[4] =  2.803947977383;
        DF[5] =  -1.169416478137e-15;
        DF[6] =  -0.9043475313924;
        
    }
    
    private void IIR_05_65_250SPS(){
         
        NF[0] =  0.179516478347;
        NF[1] =  0;  
        NF[2] = -0.5385494350409;
        NF[3] =  0;
        NF[4] = 0.5385494350409;       
        NF[5] = 0;
        NF[6] = -0.179516478347;
        
        DF[0] =  1;
        DF[1] =  -2.869187222081;
        DF[2] =  2.975880684331;
        DF[3] =  -1.666797891076;
        DF[4] =  0.8747739395112;
        DF[5] =  -0.3062731476477;
        DF[6] =  -0.00839346201296;
        
    }
    
    private void IIR_05_65_500SPS(){
         
        NF[0] =  0.03425508502224;
        NF[1] =  0;  
        NF[2] = -0.1027652550667;
        NF[3] =  0;
        NF[4] = 0.1027652550667;       
        NF[5] = 0;
        NF[6] = -0.03425508502224;
        
        DF[0] =  1;
        DF[1] =  -4.399260782223;
        DF[2] =  8.069715648566;
        DF[3] =  -7.997069830767;
        DF[4] =  4.569085792934;
        DF[5] =  -1.429498613132;
        DF[6] =  0.1870278540999;
        
    }
    
    private void IIR_05_65_1KSPS(){
         
        NF[0] =  0.005766155361323;
        NF[1] =  0;  
        NF[2] = -0.01729846608397;
        NF[3] =  0;
        NF[4] = 0.01729846608397;       
        NF[5] = 0;
        NF[6] = -0.005766155361323;
        
        DF[0] =  1;
        DF[1] =  -5.191082955766;
        DF[2] =  11.25422962618;
        DF[3] =  -13.05794547867;
        DF[4] =  8.55958598132;
        DF[5] =  -3.006838323627;
        DF[6] =  0.4420511520187;
        
    }
    
    private void IIR_05_65_2KSPS(){
         
        NF[0] =  0.0008577354142343;
        NF[1] =  0;  
        NF[2] = -0.002573206242703;
        NF[3] =  0;
        NF[4] = 0.002573206242703;       
        NF[5] = 0;
        NF[6] = -0.0008577354142343;
        
        DF[0] =  1;
        DF[1] =  -5.594505965405;
        DF[2] =  13.05145418108;
        DF[3] =  -16.2536374822;
        DF[4] =  11.39726907763;
        DF[5] =  -4.266912571521;
        DF[6] =  0.6663327604327;
        
    }
    
    private void IIR_05_65_4KSPS(){
         
        NF[0] =  0.0001177725185983;
        NF[1] =  0;  
        NF[2] = -0.0003533175557948;
        NF[3] =  0;
        NF[4] = 0.0003533175557948;       
        NF[5] = 0;
        NF[6] = -0.0001177725185983;
        
        DF[0] =  1;
        DF[1] =  -5.797220542335;
        DF[2] =  14.00636241842;
        DF[3] =  -18.05226963405;
        DF[4] =  13.09084132682;
        DF[5] =  -5.064220712329;
        DF[6] =  0.8165071434675;
        
    }
    
    private void IIR_05_65_8KSPS(){
         
        NF[0] =  1.545733243784e-05;
        NF[1] =  0;  
        NF[2] = -4.637199731351e-05;
        NF[3] =  0;
        NF[4] = 4.637199731351e-05;       
        NF[5] = 0;
        NF[6] = -1.545733243784e-05;
        
        DF[0] =  1;
        DF[1] =  -5.89863530552;
        DF[2] =  14.49830615426;
        DF[3] =  -19.00674384178;
        DF[4] =  14.01674775412;
        DF[5] =  -5.513312072285;
        DF[6] =  0.9036373112093;
        
    }
    
    private void IIR_05_40_125SPS(){
        
        NF[0] = 0.2912338421769;
        NF[1] = 0;
        NF[2] = -0.8737015265307;
        NF[3] = 0;
        NF[4] = 0.8737015265307;       
        NF[5] = 0; 
        NF[6] = -0.2912338421769;
        
        DF[0] =  1; 
        DF[1] =  -2.149935606066;
        DF[2] =  1.049375116231;
        DF[3] =  -0.04618574225955;
        DF[4] =  0.4687775738709;
        DF[5] =  -0.2437155001289;
        DF[6] =   -0.07827794921364;
        
    }
    
    private void IIR_05_40_250SPS(){
        
        NF[0] = 0.05616238477426;
        NF[1] = 0;
        NF[2] = -0.1684871543228;
        NF[3] = 0;
        NF[4] = 0.1684871543228;       
        NF[5] = 0; 
        NF[6] = -0.05616238477426;
        
        DF[0] =  1; 
        DF[1] =  -4.039588577246;
        DF[2] =  6.762259608584;
        DF[3] =  -6.159223598989;
        DF[4] =  3.311988596231;
        DF[5] =  -0.9972539700135;
        DF[6] =   0.1218188643436;
        
    }
    
     private void IIR_05_40_500SPS(){
        
        NF[0] = 0.009853425385308;
        NF[1] = 0;
        NF[2] = -0.02956027615592;
        NF[3] = 0;
        NF[4] = 0.02956027615592;       
        NF[5] = 0; 
        NF[6] = -0.009853425385308;
        
        DF[0] =  1; 
        DF[1] =  -5.007967649796;
        DF[2] =  10.48221355455;
        DF[3] =  -11.76433667406;
        DF[4] =  7.480442929736;
        DF[5] =  -2.556885366478;
        DF[6] =   0.3665332263403;
        
    }
     
     private void IIR_05_40_1KSPS(){
        
        NF[0] = 0.00151316675363;
        NF[1] = 0;
        NF[2] = -0.004539500260891;
        NF[3] = 0;
        NF[4] = 0.004539500260891;       
        NF[5] = 0; 
        NF[6] = -0.00151316675363;
        
        DF[0] =  1; 
        DF[1] =  -5.502661683637;
        DF[2] =  12.63139612221;
        DF[3] =  -15.48608738922;
        DF[4] =  10.69658137539;
        DF[5] =  -3.947176407597;
        DF[6] =   0.6079479832453;
        
    }
    
     private void IIR_05_40_2KSPS(){
        
        NF[0] = 0.0002117857743044;
        NF[1] = 0;
        NF[2] = -0.0006353573229131;
        NF[3] = 0;
        NF[4] = 0.0006353573229131;       
        NF[5] = 0; 
        NF[6] = -0.0002117857743044;
        
        DF[0] =  1; 
        DF[1] =  -5.751402987907;
        DF[2] =  13.78748872938;
        DF[3] =  -17.63413298839;
        DF[4] =  12.69150198297;
        DF[5] =  -4.87354497015;
        DF[6] =   0.7800902341086;
        
    }
     
     private void IIR_05_40_4KSPS(){
        
        NF[0] = 2.80884806505e-05;
        NF[1] = 0;
        NF[2] = -8.426544195151e-05;
        NF[3] = 0;
        NF[4] = 8.426544195151e-05;       
        NF[5] = 0; 
        NF[6] = -2.80884806505e-05;
        
        DF[0] =  1; 
        DF[1] =  -5.875781899206;
        DF[2] =  14.38664435791;
        DF[3] =  -18.78852148467;
        DF[4] =  13.80351766362;
        DF[5] =  -5.409138808558;
        DF[6] =   0.8832801709043;
        
    }
     
     private void IIR_05_40_8KSPS(){
        
        NF[0] = 3.619109210419e-06;
        NF[1] = 0;
        NF[2] = -1.085732763126e-05;
        NF[3] = 0;
        NF[4] = 1.085732763126e-05;       
        NF[5] = 0; 
        NF[6] = -3.619109210419e-06;
        
        DF[0] =  1; 
        DF[1] =  -5.937919399868;
        DF[2] =  14.69154494682;
        DF[3] =  -19.38695530436;
        DF[4] =  14.39079025433;
        DF[5] =  -5.697297383521;
        DF[6] =   0.9398368865939;
        
    }
     
    private void IIR_05_20_125SPS(){
        
        NF[0] = 0.05446272397108;
        NF[1] = 0;  
        NF[2] = -0.1633881719133;
        NF[3] = 0;
        NF[4] = 0.1633881719133;       
        NF[5] = 0;
        NF[6] = -0.05446272397108;
        
        DF[0] =  1;
        DF[1] =  -4.035000434058;
        DF[2] =  6.7669061493;
        DF[3] =  -6.192294851923;
        DF[4] =  3.350140394464;
        DF[5] =  -1.015419873759;    
        DF[6] =  0.1256760308921;
        
    }
    
    private void IIR_05_20_250SPS(){
        
        NF[0] = 0.009530483219676;
        NF[1] = 0;  
        NF[2] = -0.02859144965903;
        NF[3] = 0;
        NF[4] = 0.02859144965903;       
        NF[5] = 0;
        NF[6] = -0.009530483219676;
        
        DF[0] =  1;
        DF[1] =  -5.012117534896;
        DF[2] =  10.50594981437;
        DF[3] =  -11.81439380427;
        DF[4] =  7.530761827078;
        DF[5] =  -2.581528031578;    
        DF[6] =  0.371327892275;
        
    }
    
    private void IIR_05_20_500SPS(){
        
        NF[0] = 0.001460477432912;
        NF[1] = 0;  
        NF[2] = -0.004381432298736;
        NF[3] = 0;
        NF[4] = 0.004381432298736;       
        NF[5] = 0;
        NF[6] = -0.001460477432912;
        
        DF[0] =  1;
        DF[1] =  -5.506711995898;
        DF[2] =  12.65172137807;
        DF[3] =  -15.52654408635;
        DF[4] =  10.73658286157;
        DF[5] =  -3.966858017298;    
        DF[6] =  0.6118098630404;
        
    }
    
    private void IIR_05_20_1KSPS(){
        
        NF[0] = 0.0002041459071188;
        NF[1] = 0;  
        NF[2] = -0.0006124377213563;
        NF[3] = 0;
        NF[4] = 0.0006124377213563;       
        NF[5] = 0;
        NF[6] = -0.0002041459071188;
        
        DF[0] =  1;
        DF[1] =  -5.753969749412;
        DF[2] =  13.80024421549;
        DF[3] =  -17.65945267622;
        DF[4] =  12.71660125369;
        DF[5] =  -4.88597255953;    
        DF[6] =  0.7825495160401;
        
    }
    
    private void IIR_05_20_2KSPS(){
        
        NF[0] = 2.705587097926e-05;
        NF[1] = 0;  
        NF[2] = -8.116761293779e-05;
        NF[3] = 0;
        NF[4] = 8.116761293779e-05;       
        NF[5] = 0;
        NF[6] = -2.705587097926e-05;
        
        DF[0] =  1;
        DF[1] =  -5.877206850694;
        DF[2] =  14.3937367126;
        DF[3] =  -18.80263768279;
        DF[4] =  13.81756200745;
        DF[5] =  -5.416123562334;    
        DF[6] =  0.8846693757671;
        
    }
    
    private void IIR_05_20_4KSPS(){
        
        NF[0] = 3.484754287502e-06;
        NF[1] = 0;  
        NF[2] = -1.045426286251e-05;
        NF[3] = 0;
        NF[4] = 1.045426286251e-05;       
        NF[5] = 0;
        NF[6] = -3.484754287502e-06;
        
        DF[0] =  1;
        DF[1] =  -5.938668064205;
        DF[2] =  14.69527852441;
        DF[3] =  -19.39440255176;
        DF[4] =  14.39821719763;
        DF[5] =  -5.701000516568;    
        DF[6] =  0.9405754104908;
        
    }
    
    private void IIR_05_20_8KSPS(){
        
        NF[0] = 4.422395287232e-07;
        NF[1] = 0;  
        NF[2] = -1.32671858617e-06;
        NF[3] = 0;
        NF[4] = 1.32671858617e-06;       
        NF[5] = 0;
        NF[6] = -4.422395287232e-07;
        
        DF[0] =  1;
        DF[1] =  -5.969351359534;
        DF[2] =  14.8472428938;
        DF[3] =  -19.69545406387;
        DF[4] =  14.69641842709;
        DF[5] =  -5.848689440121;    
        DF[6] =  0.9698335426277;
        
    }
    
    private void IIR_2_65_125SPS(){
        
        NF[0] =  0.9186724341989;
        NF[1] =  0;  
        NF[2] =  -2.756017302597;
        NF[3] =  0;
        NF[4] =  2.756017302597;       
        NF[5] =  0;
        NF[6] =  -0.9186724341989;
        
        DF[0] =  1;
        DF[1] =  0.169549909223;
        DF[2] =  -2.816379925953;
        DF[3] =  -0.3103982696316;
        DF[4] =  2.662583839051;
        DF[5] =  0.1430932370718;
        DF[6] =  -0.8439590364355;
        
    }
    
    private void IIR_2_65_250SPS(){
        
        NF[0] =  0.1698258468569;
        NF[1] =  0;  
        NF[2] =  -0.5094775405707;
        NF[3] =  0;
        NF[4] =  0.5094775405707;       
        NF[5] =  0;
        NF[6] =  -0.1698258468569;
        
        DF[0] =  1;
        DF[1] =  -2.821734605463;
        DF[2] =  2.930468579725;
        DF[3] =  -1.702661969847;
        DF[4] =  0.9063373196691;
        DF[5] =  -0.3101291985499;
        DF[6] =  -0.002094643158845;
        
    }
    
    private void IIR_2_65_500SPS(){
        
        NF[0] =  0.03232054543789;
        NF[1] =  0;  
        NF[2] =  -0.09696163631367;
        NF[3] =  0;
        NF[4] =  0.09696163631367;       
        NF[5] =  0;
        NF[6] =  -0.03232054543789;
        
        DF[0] =  1;
        DF[1] =  -4.398965204498;
        DF[2] =  8.095717822694;
        DF[3] =  -8.073908635408;
        DF[4] =  4.65195893902;
        DF[5] =  -1.469877270362;
        DF[6] =  0.1950788334642;
        
    }
    
    private void IIR_2_65_1KSPS(){
        
        NF[0] =  0.005414679997367;
        NF[1] =  0;  
        NF[2] =  -0.0162440399921;
        NF[3] =  0;
        NF[4] =  0.0162440399921;       
        NF[5] =  0;
        NF[6] =  -0.005414679997367;
        
        DF[0] =  1;
        DF[1] =  -5.199440944795;
        DF[2] =  11.29851028477;
        DF[3] =  -13.14833213952;
        DF[4] =  8.64955800042;
        DF[5] =  -3.050941163737;
        DF[6] =  0.4506460571572;
        
    }
    
    private void IIR_2_65_2KSPS(){
        
        NF[0] =  0.0008026882678495;
        NF[1] =  0;  
        NF[2] =  -0.002408064803548;
        NF[3] =  0;
        NF[4] =  0.002408064803548;       
        NF[5] =  0;
        NF[6] =  -0.0008026882678495;
        
        DF[0] =  1;
        DF[1] =  -5.601179223065;
        DF[2] =  13.08473929381;
        DF[3] =  -16.31972923116;
        DF[4] =  11.46263283964;
        DF[5] =  -4.299138500859;
        DF[6] =  0.6726748233828;
        
    }
    
    private void IIR_2_65_4KSPS(){
        
        NF[0] =  0.0001099916563646;
        NF[1] =  0;  
        NF[2] =  -0.0003299749690939;
        NF[3] =  0;
        NF[4] =  0.0003299749690939;       
        NF[5] =  0;
        NF[6] =  -0.0001099916563646;
        
        DF[0] =  1;
        DF[1] =  -5.801228461919;
        DF[2] =  14.02628375123;
        DF[3] =  -18.09184325955;
        DF[4] =  13.13011792094;
        DF[5] =  -5.083698739962;
        DF[6] =  0.8203687892975;
        
    }
    
    private void IIR_2_65_8KSPS(){
        
        NF[0] =  1.442035795415e-05;
        NF[1] =  0;  
        NF[2] =  -4.326107386244e-05;
        NF[3] =  0;
        NF[4] =  4.326107386244e-05;       
        NF[5] =  0;
        NF[6] =  -1.442035795415e-05;
        
        DF[0] =  1;
        DF[1] =  -5.900813253463;
        DF[2] =  14.50915351671;
        DF[3] =  -19.02835027244;
        DF[4] =  14.03826264548;
        DF[5] =  -5.524022272565;
        DF[6] =  0.9057696362788;
        
    }
    
    private void IIR_2_40_125SPS(){
        
        NF[0] = 0.2652270549791;
        NF[1] = 0;  
        NF[2] = -0.7956811649373;
        NF[3] = 0;
        NF[4] = 0.7956811649373;       
        NF[5] = 0;
        NF[6] = -0.2652270549791;
        
        DF[0] =  1;
        DF[1] =  -2.027266178539;
        DF[2] =  0.9697173119092;
        DF[3] =  -0.1475073473385;
        DF[4] =  0.4840110691703;
        DF[5] =  -0.2150509705025;
        DF[6] =  -0.06152167442472;
        
    }
    
    private void IIR_2_40_250SPS(){
        
        NF[0] = 0.05114810063942;
        NF[1] = 0;  
        NF[2] = -0.1534443019183;
        NF[3] = 0;
        NF[4] = 0.1534443019183;       
        NF[5] = 0;
        NF[6] = -0.05114810063942;
        
        DF[0] =  1;
        DF[1] =  -4.025482681024;
        DF[2] =  6.775625720009;
        DF[3] =  -6.257750382527;
        DF[4] =  3.426590346399;
        DF[5] =  -1.052567934669;
        DF[6] =  0.1336447707714;
        
    }
    
    private void IIR_2_40_500SPS(){
        
        NF[0] = 0.008903136788806;
        NF[1] = 0;  
        NF[2] = -0.02670941036642;
        NF[3] = 0;
        NF[4] = 0.02670941036642;       
        NF[5] = 0;
        NF[6] = -0.008903136788806;
        
        DF[0] =  1;
        DF[1] =  -5.020355834273;
        DF[2] =  10.55332809504;
        DF[3] =  -11.91471978829;
        DF[4] =  7.632072974672;
        DF[5] =  -2.631416359776;
        DF[6] =  0.3810922272684;
        
    }
    
    private void IIR_2_40_1KSPS(){
        
        NF[0] = 0.001358528084502;
        NF[1] = 0;  
        NF[2] = -0.004075584253505;
        NF[3] = 0;
        NF[4] = 0.004075584253505;       
        NF[5] = 0;
        NF[6] = -0.001358528084502;
        
        DF[0] =  1;
        DF[1] =  -5.514803460178;
        DF[2] =  12.69239658572;
        DF[3] =  -15.60765706848;
        DF[4] =  10.81694708821;
        DF[5] =  -4.006488136237;
        DF[6] =  0.6196050161094;
        
    }
    
    private void IIR_2_40_2KSPS(){
        
        NF[0] = 0.0001894008303267;
        NF[1] = 0;  
        NF[2] = -0.0005682024909802;
        NF[3] = 0;
        NF[4] = 0.0005682024909802;       
        NF[5] = 0;
        NF[6] = -0.0001894008303267;
        
        DF[0] =  1;
        DF[1] =  -5.759102011295;
        DF[2] =  13.82577138526;
        DF[3] =  -17.71017118727;
        DF[4] =  12.76692740074;
        DF[5] =  -4.910916590376;
        DF[6] =  0.7874910033816;
        
    }
    
    private void IIR_2_40_4KSPS(){
        
        NF[0] = 2.506572968614e-05;
        NF[1] = 0;  
        NF[2] = -7.519718905841e-05;
        NF[3] = 0;
        NF[4] = 7.519718905841e-05;       
        NF[5] = 0;
        NF[6] = -2.506572968614e-05;
        
        DF[0] =  1;
        DF[1] =  -5.880056587437;
        DF[2] =  14.40792703154;
        DF[3] =  -18.83089442044;
        DF[4] =  13.84568839381;
        DF[5] =  -5.430118710897;
        DF[6] =  0.8874542934223;
        
    }
    
    private void IIR_2_40_8KSPS(){
        
        NF[0] = 3.22600551455e-06;
        NF[1] = 0;  
        NF[2] = -9.678016543649e-06;
        NF[3] = 0;
        NF[4] = 9.678016543649e-06;       
        NF[5] = 0;
        NF[6] = -3.22600551455e-06;
        
        DF[0] =  1;
        DF[1] =  -5.940165371505;
        DF[2] =  14.70274729845;
        DF[3] =  -19.40930376674;
        DF[4] =  14.41308131739;
        DF[5] =  -5.708413670678;
        DF[6] =  0.9420541930856;
        
    }
    
    private void IIR_2_20_125SPS(){
      
        NF[0] = 0.04485569043358;
        NF[1] = 0;  
        NF[2] = -0.1345670713007;
        NF[3] = 0;
        NF[4] = 0.1345670713007;       
        NF[5] = 0;
        NF[6] = -0.04485569043358;
        
        DF[0] =  1;
        DF[1] =  -4.005053739023;
        DF[2] =  6.790748301473;
        DF[3] =  -6.38583159616;
        DF[4] =  3.580081428132;
        DF[5] =  -1.130127224769;
        DF[6] =  0.1506702960696;
        
    }
    
    private void IIR_2_20_250SPS(){
      
        NF[0] = 0.007721931222771;
        NF[1] = 0;  
        NF[2] = -0.02316579366831;
        NF[3] = 0;
        NF[4] = 0.02316579366831;       
        NF[5] = 0;
        NF[6] = -0.007721931222771;
        
        DF[0] =  1;
        DF[1] =  -5.03658218624;
        DF[2] =  10.6476953455;
        DF[3] =  -12.11621180377;
        DF[4] =  7.837413196471;
        DF[5] =  -2.733645585296;
        DF[6] =  0.4013417274526;
        
    }
    
    private void IIR_2_20_500SPS(){
      
        NF[0] = 0.001168105334547;
        NF[1] = 0;  
        NF[2] = -0.00350431600364;
        NF[3] = 0;
        NF[4] = 0.00350431600364;       
        NF[5] = 0;
        NF[6] = -0.001168105334547;
        
        DF[0] =  1;
        DF[1] =  -5.530949180194;
        DF[2] =  12.77384443564;
        DF[3] =  -15.77068359872;
        DF[4] =  10.97913076853;
        DF[5] =  -4.086827413712;
        DF[6] =  0.6354851916294;
        
    }
    
    private void IIR_2_20_1KSPS(){
      
        NF[0] = 0.0001619992981255;
        NF[1] = 0;  
        NF[2] = -0.0004859978943766;
        NF[3] = 0;
        NF[4] = 0.0004859978943766;       
        NF[5] = 0;
        NF[6] = -0.0001619992981255;
        
        DF[0] =  1;
        DF[1] =  -5.769361423314;
        DF[2] =  13.87689042234;
        DF[3] =  -17.81192545846;
        DF[4] =  12.8680922274;
        DF[5] =  -4.961162136432;
        DF[6] =  0.7974663719948;
        
    }
    
    private void IIR_2_20_2KSPS(){
      
        NF[0] = 2.137776770293e-05;
        NF[1] = 0;  
        NF[2] = -6.413330310879e-05;
        NF[3] = 0;
        NF[4] = 6.413330310879e-05;       
        NF[5] = 0;
        NF[6] = -2.137776770293e-05;
        
        DF[0] =  1;
        DF[1] =  -5.885755388188;
        DF[2] =  14.43633010398;
        DF[3] =  -18.88750538859;
        DF[4] =  13.90209229555;
        DF[5] =  -5.45821188344;
        DF[6] =  0.8930502607542;
        
    }
    
    private void IIR_2_20_4KSPS(){
      
        NF[0] = 2.747224589366e-06;
        NF[1] = 0;  
        NF[2] = -8.241673768099e-06;
        NF[3] = 0;
        NF[4] = 8.241673768099e-06;       
        NF[5] = 0;
        NF[6] = -2.747224589366e-06;
        
        DF[0] =  1;
        DF[1] =  -5.943159899694;
        DF[2] =  14.71769132202;
        DF[3] =  -19.43913309617;
        DF[4] =  14.44285053651;
        DF[5] =  -5.723267573579;
        DF[6] =  0.9450187109167;
        
    }
    
    private void IIR_2_20_8KSPS(){
      
        NF[0] = 3.482403794893e-07;
        NF[1] = 0;  
        NF[2] = -1.044721138468e-06;
        NF[3] = 0;
        NF[4] = 1.044721138468e-06;       
        NF[5] = 0;
        NF[6] = -3.482403794893e-07;
        
        DF[0] =  1;
        DF[1] =  -5.971652227237;
        DF[2] =  14.85873449755;
        DF[3] =  -19.7184115331;
        DF[4] =  14.71934989792;
        DF[5] =  -5.860142050544;
        DF[6] =  0.9721214154114;
        
    }
    
     public void ResetNotch(){
        
        NF_Notch[0] = 1;
        NF_Notch[1] = 0;  
        NF_Notch[2] = 0;
        NF_Notch[3] = 0;
        NF_Notch[4] = 0;       
        NF_Notch[5] = 0;
        NF_Notch[6] = 0;
        
        DF_Notch[0] =  1;
        DF_Notch[1] =  0;
        DF_Notch[2] =  0;
        DF_Notch[3] =  0;
        DF_Notch[4] =  0;
        DF_Notch[5] =  0;
        DF_Notch[6] =  0;
        
    }
     
     public void SetNotch_125SPS(){
         NF_Notch[0] = 0.8175824025905;
         NF_Notch[1] = 3.988767567701;  
         NF_Notch[2] = 8.939460008311;
         NF_Notch[3] = 11.49386294364;
         NF_Notch[4] = 8.939460008311;       
         NF_Notch[5] = 3.988767567701;
         NF_Notch[6] = 0.8175824025905;
        
         DF_Notch[0] =  1;
         DF_Notch[1] =  4.552292625804;
         DF_Notch[2] =  9.521459784101;
         DF_Notch[3] =  11.43974887193;
         DF_Notch[4] =  8.324184756821;
         DF_Notch[5] =  3.479356581302;
         DF_Notch[6] =  0.6684402808819;
     }
     
     public void SetNotch_250SPS(){
         NF_Notch[0] = 0.9043187344848;
         NF_Notch[1] = -1.678819565687;  
         NF_Notch[2] = 3.751835906173;
         NF_Notch[3] = -3.571930779859;
         NF_Notch[4] = 3.751835906173;       
         NF_Notch[5] = -1.678819565687;
         NF_Notch[6] = 0.9043187344848;
        
         DF_Notch[0] =  1;
         DF_Notch[1] =  -1.794262584793;
         DF_Notch[2] =  3.872744259924;
         DF_Notch[3] =  -3.566265590484;
         DF_Notch[4] =  3.621772661108;
         DF_Notch[5] =  -1.569041735955;
         DF_Notch[6] =  0.8177923602828;
     }
     
     public void SetNotch_500SPS(){
         NF_Notch[0] = 0.9509718879234;
         NF_Notch[1] = -4.617572789989;  
         NF_Notch[2] = 10.32666567385;
         NF_Notch[3] = -13.26734617205;
         NF_Notch[4] = 10.32666567385;       
         NF_Notch[5] = -4.617572789989;
         NF_Notch[6] = 0.9509718879234;
        
         DF_Notch[0] =  1;
         DF_Notch[1] =  -4.774287021015;
         DF_Notch[2] =  10.49847986078;
         DF_Notch[3] =  -13.26345558518;
         DF_Notch[4] =  10.15244773137;
         DF_Notch[5] =  -4.464749145826;
         DF_Notch[6] =  0.9043475313924;
     }
     
     public void SetNotch_1KSPS(){
         NF_Notch[0] = 0.9751798116348;
         NF_Notch[1] = -5.565146087028;  
         NF_Notch[2] = 13.5119121908;
         NF_Notch[3] = -17.84298137539;
         NF_Notch[4] = 13.5119121908;       
         NF_Notch[5] = -5.565146087028;
         NF_Notch[6] = 0.9751798116348;
        
         DF_Notch[0] =  1;
         DF_Notch[1] =  -5.658981850005;
         DF_Notch[2] =  13.62480145326;
         DF_Notch[3] =  -17.84180950183;
         DF_Notch[4] =  13.39840688659;
         DF_Notch[5] =  -5.472482197613;
         DF_Notch[6] =  0.9509756650163;
     }
     
     public void SetNotch_2KSPS(){
         NF_Notch[0] = 0.9875121748696;
         NF_Notch[1] = -5.852241086115;  
         NF_Notch[2] = 14.52314529534;
         NF_Notch[3] = -19.31681809525;
         NF_Notch[4] = 14.52314529534;       
         NF_Notch[5] = -5.852241086115;
         NF_Notch[6] = 0.9875121748696;
        
         DF_Notch[0] =  1;
         DF_Notch[1] =  -5.901423380444;
         DF_Notch[2] =  14.58390179689;
         DF_Notch[3] =  -19.31651003752;
         DF_Notch[4] =  14.46223284803;
         DF_Notch[5] =  -5.803366849518;
         DF_Notch[6] =  0.9751802955156;
     }
     
     public void SetNotch_4KSPS(){
         NF_Notch[0] = 0.993736502354;
         NF_Notch[1] = -5.944068198937;  
         NF_Notch[2] = 14.83275723288;
         NF_Notch[3] = -19.76485084083;
         NF_Notch[4] = 14.83275723288;       
         NF_Notch[5] = -5.944068198937;
         NF_Notch[6] = 0.993736502354;
        
         DF_Notch[0] =  1;
         DF_Notch[1] =  -5.969005846157;
         DF_Notch[2] =  14.86380328105;
         DF_Notch[3] =  -19.76477261951;
         DF_Notch[4] =  14.80167195331;
         DF_Notch[5] =  -5.919208773034;
         DF_Notch[6] =  0.9875122361107;
     }
     
     public void SetNotch_8KSPS(){
         NF_Notch[0] = 0.9968633356971;
         NF_Notch[1] = -5.976576114458;  
         NF_Notch[2] = 14.93454152409;
         NF_Notch[3] = -19.90965748701;
         NF_Notch[4] = 14.93454152409;       
         NF_Notch[5] = -5.976576114458;
         NF_Notch[6] = 0.9968633356971;
        
         DF_Notch[0] =  1;
         DF_Notch[1] =  -5.989103267586;
         DF_Notch[2] =  14.95017602134;
         DF_Notch[3] =  -19.90963782483;
         DF_Notch[4] =  14.91889718817;
         DF_Notch[5] =  -5.964068623509;
         DF_Notch[6] =  0.9937365100571;
     }
     
      public void SetNotch(int SampleRate){
        
        switch(SampleRate){
            case(125):SetNotch_125SPS();
                break;
            case(250):SetNotch_250SPS();
                break;    
            case(500):SetNotch_500SPS();
                break; 
            case(1000):SetNotch_1KSPS();
                break;
            case(2000):SetNotch_2KSPS();
                break;
            case(4000):SetNotch_4KSPS();
                break;
            case(8000):SetNotch_8KSPS();
                break;    
        }
    }
    
    
    public void SetFilter(int n, int SampleRate){
    
        switch(n){
            case(0): NoFilter();
                break;
            case(1): 
                switch(SampleRate){
                    case(125): IIR_05_65_125SPS();
                        break;
                    case(250): IIR_05_65_250SPS();
                        break;
                    case(500): IIR_05_65_500SPS();
                        break;
                    case(1000): IIR_05_65_1KSPS();
                        break;
                    case(2000): IIR_05_65_2KSPS();
                        break;
                    case(4000): IIR_05_65_4KSPS();
                        break;
                    case(8000): IIR_05_65_8KSPS();
                        break;
                }  
                break;
            case(2): switch(SampleRate){
                    case(125): IIR_05_40_125SPS();
                        break;
                    case(250): IIR_05_40_250SPS();
                        break;
                    case(500): IIR_05_40_500SPS();
                        break;
                    case(1000): IIR_05_40_1KSPS();
                        break;
                    case(2000): IIR_05_40_2KSPS();
                        break;
                    case(4000): IIR_05_40_4KSPS();
                        break;
                    case(8000): IIR_05_40_8KSPS();
                        break;
                }  
                break;
            case(3): switch(SampleRate){
                    case(125): IIR_05_20_125SPS();
                        break;
                    case(250): IIR_05_20_250SPS();
                        break;
                    case(500): IIR_05_20_500SPS();
                        break;
                    case(1000): IIR_05_20_1KSPS();
                        break;
                    case(2000): IIR_05_20_2KSPS();
                        break;
                    case(4000): IIR_05_20_4KSPS();
                        break;
                    case(8000): IIR_05_20_8KSPS();
                        break;
                }  
                break;
            case(4): switch(SampleRate){
                    case(125): IIR_2_65_125SPS();
                        break;
                    case(250): IIR_2_65_250SPS();
                        break;
                    case(500): IIR_2_65_500SPS();
                        break;
                    case(1000): IIR_2_65_1KSPS();
                        break;
                    case(2000): IIR_2_65_2KSPS();
                        break;
                    case(4000): IIR_2_65_4KSPS();
                        break;
                    case(8000): IIR_2_65_8KSPS();
                        break;
                }  
                break;
            case(5): switch(SampleRate){
                    case(125): IIR_2_40_125SPS();
                        break;
                    case(250): IIR_2_40_250SPS();
                        break;
                    case(500): IIR_2_40_500SPS();
                        break;
                    case(1000): IIR_2_40_1KSPS();
                        break;
                    case(2000): IIR_2_40_2KSPS();
                        break;
                    case(4000): IIR_2_40_4KSPS();
                        break;
                    case(8000): IIR_2_40_8KSPS();
                        break;
                }  
                break;
            case(6): switch(SampleRate){
                    case(125): IIR_2_20_125SPS();
                        break;
                    case(250): IIR_2_20_250SPS();
                        break;
                    case(500): IIR_2_20_500SPS();
                        break;
                    case(1000): IIR_2_20_1KSPS();
                        break;
                    case(2000): IIR_2_20_2KSPS();
                        break;
                    case(4000): IIR_2_20_4KSPS();
                        break;
                    case(8000): IIR_2_20_8KSPS();
                        break;
                }  
                break;    
        }
        
    }
    
}