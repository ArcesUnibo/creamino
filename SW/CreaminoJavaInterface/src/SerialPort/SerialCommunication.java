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
package SerialPort;

import Filter.DigitalFilter;
import creaminointerface.FXMLDocumentController;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.event.ActionEvent;
import javafx.geometry.Bounds;
import javafx.scene.Node;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.paint.Color;
import javafx.util.Duration;
import jssc.SerialPort;
import static jssc.SerialPort.PURGE_RXCLEAR;
import static jssc.SerialPort.PURGE_TXCLEAR;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;
import jssc.SerialPortList;
import jssc.SerialPortTimeoutException;

/**
 *
 * @author Matteo Chiesi - University of Bologna
 */
public class SerialCommunication implements SerialPortEventListener{
    
   /*This class use the JSSC library to implement a communication over the
    virtual COM port */
    
   /*Class variables*/ 
   private static final String ReferenceWord = "Ready";
   String ConnectingWord;
   static SerialPort serialPort;
   private byte[] DataBuffer = new byte[1000000];
   public double[] Word = new double[FXMLDocumentController.MAXCHANNELS];
   public double[] NotchWord = new double[FXMLDocumentController.MAXCHANNELS];
   public double[] FilteredWord = new double[FXMLDocumentController.MAXCHANNELS];
   private int GraphCount = 0;
   private int lenght, DataLength, GraphUpdateCycles;
   private FileOutputStream outputStream;
   private FileOutputStream outputStreamTrigger;
   String fileName = "temp.bin";
   private final int PGA = 1;
   DigitalFilter IIR = new DigitalFilter(FXMLDocumentController.MAXCHANNELS);
   private boolean start;
   private double REF;
   private final ConcurrentLinkedQueue<Number>[] BufferQueue =  new ConcurrentLinkedQueue[FXMLDocumentController.MAXCHANNELS]; 
   
   private int xSeriesData = 0;
   public List<XYChart.Series> seriesList;  
   
   private final FXMLDocumentController Controller; 
   private final Timeline animation;
   private static int Remove;
    
   private Node chartArea;
   private Bounds chartAreaBounds;
   private double RefactorMovableLine;
    
    
   public SerialCommunication(FXMLDocumentController Controller) {
       this.Controller = Controller; 
       this.seriesList = new ArrayList<>();
       IIR.ResetNotch();
        
       Controller.Chart.setCreateSymbols(false);
       Controller.Chart.getData().clear();
        
       for(int i=0;i<FXMLDocumentController.MAXCHANNELS;i++){
            //Create a list of Series -> each series is connected to a chart
            XYChart.Series series = new XYChart.Series();
            series.setName("series" + i);
            seriesList.add(series);
            Controller.Chart.getData().add(seriesList.get(i));
            //Create a new Queue for each list
            BufferQueue[i] = new ConcurrentLinkedQueue(); 
        }
        
        //-- Prepare Executor Services
        
        animation = new Timeline();
        animation.getKeyFrames().add(new KeyFrame(Duration.millis(100), (ActionEvent actionEvent) -> {
            addDataToSeries();
        }));
        
        animation.setCycleCount(Animation.INDEFINITE);
        
    }
    
 /*** @return *****************************************************/

    //Check devices connect to COM Ports
    public String[] CheckPorts(){
        String[] portNames = SerialPortList.getPortNames();
        return portNames; 
     }
        
     /**
     * @param SelectedPort*
     * @return ****************************************************/
    //Checks if the device is programmed properly and Starts the AD Conversion
    public boolean StartADS(String SelectedPort){
        //The ADS starts to send data to Arduino
            boolean Check = false;
            serialPort = new SerialPort(SelectedPort);
            try {
                serialPort.openPort();
                serialPort.setParams(SerialPort.BAUDRATE_256000, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
                serialPort.purgePort(PURGE_RXCLEAR | PURGE_TXCLEAR);
                serialPort.writeString("Connect");
                
                ConnectingWord = serialPort.readString(5, 5000);                  
                System.out.println(ConnectingWord);
                    if(ConnectingWord.equals(ReferenceWord))
                        Check = true;
          
                serialPort.writeBytes(FXMLDocumentController.InitParam);
                    
                serialPort.closePort();
           }
            catch (SerialPortException | SerialPortTimeoutException ex ) {
            System.out.println("The serial port is currently busy");
        }
       
        chartArea = Controller.Chart.lookup(".chart-plot-background");
        chartAreaBounds = chartArea.localToScene(chartArea.getBoundsInLocal());
            
       return Check;     
    }           
    
 /**
     * @param TextField*    
     * @param SelectedPort****************************************************/    
    
// Initialization -- Open the Native USB port and the output file
    public void initialize(String TextField, String SelectedPort){
       //Create the output stream to save the data;
        try {
            if(!TextField.equals("")){
               outputStream = new FileOutputStream(TextField + ".bin");
               outputStreamTrigger = new FileOutputStream("Trigger.bin");}
            else{
               outputStream = new FileOutputStream(fileName);
               outputStreamTrigger = new FileOutputStream("Trigger.bin");}
        } catch(IOException e) {
            System.err.println(e);
        }
       //Initialize the serial port
       serialPort = new SerialPort(SelectedPort);
        
       try {
            serialPort.openPort();
            serialPort.setParams(SerialPort.BAUDRATE_256000, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
            //Preparing a mask. In a mask, we need to specify the types of events that we want to track.
            //Well, for example, we need to know what came some data, thus in the mask must have the
            //following value: MASK_RXCHAR. If we, for example, still need to know about changes in states 
            //of lines CTS and DSR, the mask has to look like this: SerialPort.MASK_RXCHAR + SerialPort.MASK_CTS + SerialPort.MASK_DSR
            int mask = SerialPort.MASK_RXCHAR;
            //Set the prepared mask
            serialPort.setEventsMask(mask);
            //Reset the device
            resetArduino();
            //Empty the SerialPort
            serialPort.readBytes();
            //Empty the buffer
            for(int i=0;i<FXMLDocumentController.MAXCHANNELS;i++){
                BufferQueue[i].clear();
            }
            
            //Synchronize the data with the graphs 
            SyncChannels();
                     
            //Add an interface through which we will receive information about events
            serialPort.addEventListener(this);
            
        }
        catch (SerialPortException ex) {
            System.out.println(ex);
        }
       prepareTimeline();
    }
  /*******************************************************/
    
   
/*******************************************************/
    //Close the native USB port
    public synchronized void closePort(){
        animation.pause();
         
        try {
            if (serialPort != null) {
                serialPort.removeEventListener();
                try {
                    Thread.sleep(100);
                } catch (InterruptedException ex) {
                    Logger.getLogger(SerialCommunication.class.getName()).log(Level.SEVERE, null, ex);
                }
                
                serialPort.writeString("Disconnect\0");
                try {
                    Thread.sleep(100);
                } catch (InterruptedException ex) {
                    Logger.getLogger(SerialCommunication.class.getName()).log(Level.SEVERE, null, ex);
                }
                serialPort.purgePort(PURGE_RXCLEAR | PURGE_TXCLEAR);
                serialPort.closePort();
            }
        }
        catch (SerialPortException ex) {
            System.out.println(ex);
        }
        write();  
        Remove = 0;
        GraphCount =0;
        
         for(int j=0;j<FXMLDocumentController.CHANNELS;j++){    
            seriesList.get(j).getData().remove(0, seriesList.get(j).getData().size());   
        }
        
    }
        
        public void resetArduino() {
       try {
           serialPort.setDTR(true);
           try {
               Thread.sleep(100);
           } catch (InterruptedException ex) {
               Logger.getLogger(SerialCommunication.class.getName()).log(Level.SEVERE, null, ex);
           }
           serialPort.setDTR(false);
           try {
               Thread.sleep(100);
           } catch (InterruptedException ex) {
               Logger.getLogger(SerialCommunication.class.getName()).log(Level.SEVERE, null, ex);
           }
       } catch (SerialPortException ex) {
           Logger.getLogger(SerialCommunication.class.getName()).log(Level.SEVERE, null, ex);
       }              
    }
        
        
    /**
     * @param event*****************************************************/    
    @Override
    public synchronized void serialEvent(SerialPortEvent event) {
        if(event.isRXCHAR() && event.getEventValue() > 0){
            try {
                
                    lenght = serialPort.getInputBufferBytesCount();
                    DataLength =  lenght - lenght%((FXMLDocumentController.CHANNELS*FXMLDocumentController.bytesPerChannel+2));
                    GraphUpdateCycles = DataLength/((FXMLDocumentController.CHANNELS*FXMLDocumentController.bytesPerChannel+2));
                    
                    if(GraphUpdateCycles>0)
                    {   
                        DataBuffer = serialPort.readBytes(DataLength);
                        
                        for(int j=0;j<GraphUpdateCycles;j++){   
                            
                            try {
                                outputStream.write(DataBuffer, 0, FXMLDocumentController.CHANNELS*FXMLDocumentController.bytesPerChannel);
                                outputStreamTrigger.write(DataBuffer, 0, FXMLDocumentController.CHANNELS*FXMLDocumentController.bytesPerChannel+2);
                            } catch (Exception e) {
                                System.err.println(e.toString());
                            }
                            
                            REF = 0;
                            GraphCount++;
                                for(int i=0;i<FXMLDocumentController.CHANNELS;i++){
                                    if ((0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)]) > 127) {
                                        if (FXMLDocumentController.ADSType == 1)
                                            Word[i] = (2*2.4*(-(65536-((0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)])*256+(0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)+1])+1))/65536.0/FXMLDocumentController.AVAC/PGA))/Controller.scaleY;//
                                        else                                               
                                            Word[i] = (2*2.4*(-(16777216-((0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)])*65536+(0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)+1])*256+(0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)+2])+1))/16777216.0/FXMLDocumentController.AVAC/PGA))/Controller.scaleY;
                                    } else {
                                        if (FXMLDocumentController.ADSType == 1)
                                            Word[i] = (2*2.4*((0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)])*256+(0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)+1]))/65536.0/FXMLDocumentController.AVAC/PGA)/Controller.scaleY; //
                                        else
                                            Word[i] = (2*2.4*((0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)])*65536+(0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)+1])*256+(0xFF&DataBuffer[(i*FXMLDocumentController.bytesPerChannel)+2]))/16777216.0/FXMLDocumentController.AVAC/PGA)/Controller.scaleY; //                                        
                                    }
                                    
                                    NotchWord[i] = IIR.NotchFilteredWord(i, Word[i]);
                                    FilteredWord[i] = IIR.FilteredWord(i, NotchWord[i]); 
                                    REF += Controller.CH[i]*FilteredWord[i];
                                }
                                
                                for(int i=0;i<FXMLDocumentController.CHANNELS;i++){
                                    if(GraphCount == FXMLDocumentController.ScaleCount){
                                        BufferQueue[i].add(FilteredWord[i]-REF+ FXMLDocumentController.UpperBoundY*FXMLDocumentController.CHANNELS*2 -FXMLDocumentController.offset/2 -i*FXMLDocumentController.offset*FXMLDocumentController.full_window);
                                    }
                                }                   
                                
                                if(GraphCount == FXMLDocumentController.ScaleCount){
                                    GraphCount = 0;   
                                }
                                                             
                                if((DataBuffer[FXMLDocumentController.CHANNELS*FXMLDocumentController.bytesPerChannel]& (byte)0xF0) == (byte)0xF0 && (DataBuffer[FXMLDocumentController.CHANNELS*FXMLDocumentController.bytesPerChannel] & (byte)0xF0) == (byte)0xF0){
                                    DataBuffer=Arrays.copyOfRange(DataBuffer,(FXMLDocumentController.CHANNELS*FXMLDocumentController.bytesPerChannel+2), DataBuffer.length);
                                }
                                else{
                                    SyncChannels();
                                    System.out.println("entro qui");
                                }
                        }
                    }
                  
                    
                } catch (SerialPortException ex) {
                    Logger.getLogger(SerialCommunication.class.getName()).log(Level.SEVERE, null, ex);
            }

        }
            
    }
    
    
    public void write(){
      try{
           Thread.sleep(100);
        } catch (InterruptedException ex) {
           Logger.getLogger(SerialCommunication.class.getName()).log(Level.SEVERE, null, ex);
        }
          
      try{
        outputStream.flush();
        outputStream.close();
        outputStreamTrigger.flush();
        outputStreamTrigger.close();

        } catch (IOException e) {
            System.err.println(e);
        }
    }
   
    public void setFilter(int FilterNum, int SampleRate){
        IIR.SetFilter(FilterNum, SampleRate);
    }

      public void SetNotch(int SampleRate){
          IIR.SetNotch(SampleRate);
    }
      
       public void ResetNotch(){
          IIR.ResetNotch();
    }
      
      
      public void resetScaleCount(){
        GraphCount=0;
    }
    
     public void SyncChannels() throws SerialPortException{
            do{
                DataBuffer = serialPort.readBytes(1);
                if((DataBuffer[0] & (byte)0xF0) == (byte)0xF0){
                    DataBuffer = serialPort.readBytes(1);
                        if((DataBuffer[0] & (byte)0xF0) == (byte)0xF0){
                            start=true;}}
                            
            }while(!start);
      }

    public void SyncChannels2() throws SerialPortException{
        int i = -1;  
        do{
            i++;
            if((DataBuffer[i] & (byte)0xF0) == (byte)0xF0){
                if((DataBuffer[i+1] & (byte)0xF0) == (byte)0xF0){
                    start=true;}}
        }while(!start);
        DataBuffer=Arrays.copyOfRange(DataBuffer,(i+2), DataBuffer.length);
      }
    
     //-- Timeline gets called in the JavaFX Main thread
    public void prepareTimeline()
    {
        animation.play();
        // Every frame to take any data from queue and add to chart
    }

 
     private void addDataToSeries()
    {
    
        RefactorMovableLine = (chartAreaBounds.getMaxX() - chartAreaBounds.getMinX()) / FXMLDocumentController.MAX_DATA_POINTS;
        
        //Remove data if the interface loses some steps
        Remove = BufferQueue[0].size() - 200;
        for(int i=0;i<Remove;i++){
            for(int j=0;j<FXMLDocumentController.CHANNELS;j++)
                BufferQueue[j].remove();
        }
        
        for (int k = 0; k < 25; k++){ //-- add 25 numbers for each channels to the chart
            if (BufferQueue[0].isEmpty()){
            break;
        }
                        
        for(int j=0;j<FXMLDocumentController.CHANNELS;j++){
            if (Controller.SelectedChannelList.get(j).isSelected()){  
                seriesList.get(j).getData().add(new LineChart.Data(xSeriesData, BufferQueue[j].remove()));                                              
                    if(seriesList.get(j).getData().size() >= FXMLDocumentController.MAX_DATA_POINTS)
                        seriesList.get(j).getData().remove(0, seriesList.get(j).getData().size() - FXMLDocumentController.MAX_DATA_POINTS);
                }
            else{
                seriesList.get(j).getData().remove(0, seriesList.get(j).getData().size());   
                BufferQueue[j].remove();
            }
                
            Controller.valueMarker.setTranslateX(xSeriesData*RefactorMovableLine);
        }
            
        if(xSeriesData>= FXMLDocumentController.MAX_DATA_POINTS)
            xSeriesData = 0;
        else
            xSeriesData = xSeriesData+1;
        }
    }
    
}

