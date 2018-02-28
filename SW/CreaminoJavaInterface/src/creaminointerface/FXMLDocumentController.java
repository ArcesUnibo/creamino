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

package creaminointerface;

import SerialPort.SerialCommunication;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import static java.lang.Float.parseFloat;
import static java.lang.Math.pow;
import java.net.URL;
import java.util.List;
import java.util.ResourceBundle;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Platform;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.CheckMenuItem;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.MenuBar;
import javafx.scene.control.TextField;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.stage.Stage;
import javafx.util.Callback;

/**
 *
 * @author Matteo Chiesi - University of Bologna
 */

public class FXMLDocumentController implements Initializable {
    
    public static int CHANNELS, MAX_DATA_POINTS, MAXCHANNELS = 64;
    public int scaleX;
    public double scaleY;

    FXMLLoader fxmlLoader;
    Parent rootReference;
    Stage stageReference;
    ReferenceController ReferenceCtrl;
    
    Parent rootCalibration;
    Stage stageCalibration;
    CalibrationController CalibrationCtrl;
    
    public static int AVAC;
  
    public int CH[] = new int[MAXCHANNELS];
    public double Cal[] = new double[MAXCHANNELS];
    public int SelectedCHList_tmp[] = new int[MAXCHANNELS];
    public String CHLabel[] = new String[MAXCHANNELS];
    private SerialCommunication SerialCOM;
    private static String[] Ports;
    private static String SelectedPort;
    public static int UpperBoundY, SR, ScaleCount, offset;
    
    /* Initialization parameter 
    1) Sample Rate 
    2) ModeADS 
    3) ADSNum
    4) ChipSelect 
    5) Gain
    */
    
    public static byte InitParam[] = new byte[5];  
    public static byte ADSType;
    public static int bytesPerChannel;
    public static double full_window;
    double xShift;
    public static double LabelShiftY;
    
    @FXML
    private MenuBar Menu;
    
    @FXML
    private Button ConnectButton;
    
    @FXML
    private TextField OutputFileText, CalibrationFileText;
    
    @FXML
    public CheckMenuItem Connect, Channels1_8, Channels9_16, Channels17_24, Channels25_32, Channels33_40, Channels41_48, Channels49_56, Channels57_64, full_size;
    
    @FXML
    public ChoiceBox<String> YScaleBox, XScaleBox, COMBox, FilterChoice, ChannelsBox, SampleRateBox, ModeADSBox, ChipSelectBox, ADSModelBox, GainBox;
    
    @FXML
    public CheckBox NotchFilter;
     
    @FXML
    public LineChart Chart;
    
    @FXML
    public NumberAxis XAxis, YAxis;
     
    @FXML
    public List<CheckMenuItem> SelectedChannelList, SelectedPoints;
    
    @FXML
    public List<TextField> LabelChList;
    
    @FXML 
    public Line valueMarker;
    
    @Override
    public void initialize(URL location, ResourceBundle resources) {

        try {
            initReference();
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        try {
            initCalibration();
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        
        //Initial Condition
        
        //Configuration Words
        InitParam[0] = 0x06;    // Sample Rate of the ADS (byte that must be transmitted to the ADS)
        InitParam[1] = 0x00;    // Mode of ADS1298, default normal mode
        InitParam[2] = 2;       //Default number of ADS Scambio il 2 con l'1  
        InitParam[3] = 4;      //Chip Select
        InitParam[4] = 0x10;    //Gain
        
        //SAMPLE RATE
        SR = 500; // Sample Rate of the ADS (SPS)
        SampleRateBox.setItems(FXCollections.observableArrayList("500 SPS","1 KSPS","2 KSPS","4 KSPS","8 KSPS","16 KSPS","32 KSPS"));
        SampleRateBox.setValue("500 SPS");
        ADSType = 3;
        bytesPerChannel = 3;
        ADSModelBox.setValue("ADS1298");
        //ADS MODE
        ModeADSBox.setValue("NORMAL");
        //CHANNELS
        CHANNELS = 16; // Number of Channels used
        LabelShiftY = 24.3; // Shift the labels depending on the number of channels
        full_window = 1; // 1 - The chart is displayed on a half of the scroll pane 2 - The entire scroll pane is used 
        ChannelsBox.setValue("16");
        
        ChipSelectBox.setValue("4");
        
        //GAIN 
        GainBox.setValue("1");
        AVAC = 1;
        MAX_DATA_POINTS = 500; //Number of points visualized for each chart

        //VISUALIZATION
        scaleY = 1e-6; // Amplitude in Volts
        scaleX = 2; // Time in seconds
        YScaleBox.setValue("50uV"); 
        XScaleBox.setValue("2s"); 
        UpperBoundY = 50; // Maximum Aplitude of each channel in the graph (50 uV)       
        offset = (2*UpperBoundY);    
        ScaleCount = SR*scaleX/MAX_DATA_POINTS; //Decimation --> just for visual purpose
        for(int i=0; i<FXMLDocumentController.CHANNELS;i++){
                    LabelChList.get(i).setVisible(true);
                    LabelChList.get(i).setTranslateY(LabelShiftY*(i+1));
                }
        for(int i=FXMLDocumentController.CHANNELS; i<FXMLDocumentController.MAXCHANNELS;i++){
                    LabelChList.get(i).setVisible(false);
                }
        
        //Set Chart Axes
        XAxis.setUpperBound(MAX_DATA_POINTS);
        XAxis.setLowerBound(0);
        YAxis.setUpperBound(UpperBoundY*CHANNELS*2);
        YAxis.setLowerBound(-UpperBoundY*CHANNELS*2);

        //Select all channels
        for(int i=0;i<FXMLDocumentController.MAXCHANNELS;i++)
            SelectedChannelList.get(i).setSelected(true);
        
        Channels1_8.setSelected(true);
        Channels9_16.setSelected(true);
        Channels17_24.setSelected(true);
        Channels25_32.setSelected(true);
        Channels33_40.setSelected(true);
        Channels41_48.setSelected(true);
        Channels49_56.setSelected(true);
        Channels57_64.setSelected(true);
        
        //Hide the labels of cahnnels that are not selected.
        for(int i=0; i<FXMLDocumentController.CHANNELS;i++)
            LabelChList.get(i).setVisible(true);
        for(int i=FXMLDocumentController.CHANNELS; i<FXMLDocumentController.MAXCHANNELS;i++)
            LabelChList.get(i).setVisible(false);
        
        
        //Instance of Serial Communication Class
        SerialCOM = new SerialCommunication(this);
        //SET FILTER
        FilterChoice.setValue("2-20Hz");
        //Initialize the filter for a given Sample rate
        SerialCOM.setFilter(6,SR);
        
        
        COMBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                SelectedPort = newValue;
            }
        });
        
        ChannelsBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                for(int i=0; i<FXMLDocumentController.CHANNELS;i++){
                    LabelChList.get(i).setTranslateY(-LabelShiftY);
                }
                switch(newValue){
                    case "8"   :    
                        FXMLDocumentController.CHANNELS = 8;
                        FXMLDocumentController.InitParam[2] = 1;
                        FXMLDocumentController.LabelShiftY = 72.3;
                        FXMLDocumentController.full_window = 1;
                        break;
                    case "16"   :
                        FXMLDocumentController.CHANNELS = 16;
                        FXMLDocumentController.InitParam[2] = 2;
                        FXMLDocumentController.LabelShiftY = 24.3;
                        FXMLDocumentController.full_window = 1;
                        break;
                    case "24"   :
                        FXMLDocumentController.CHANNELS = 24;
                        FXMLDocumentController.InitParam[2] = 3;
                        FXMLDocumentController.LabelShiftY = 8.3;
                        FXMLDocumentController.full_window = 1;
                        break;
                    case "32"  :
                        FXMLDocumentController.CHANNELS = 32;
                        FXMLDocumentController.InitParam[2] = 4;
                        FXMLDocumentController.LabelShiftY = 0;
                        FXMLDocumentController.full_window = 1;
                        break;
                    case "40"  :
                        FXMLDocumentController.CHANNELS = 40;
                        FXMLDocumentController.InitParam[2] = 5;
                        FXMLDocumentController.LabelShiftY = 0;
                        FXMLDocumentController.full_window = 1.25;
                        break;
                    case "48"  :
                        FXMLDocumentController.CHANNELS = 48;
                        FXMLDocumentController.InitParam[2] = 6;
                        FXMLDocumentController.LabelShiftY = 0;
                        FXMLDocumentController.full_window = 1.5;
                        break;
                    case "56"    :
                        FXMLDocumentController.CHANNELS = 56;
                        FXMLDocumentController.InitParam[2] = 7;
                        FXMLDocumentController.LabelShiftY = 0;
                        FXMLDocumentController.full_window = 1.75;
                        break;
                    case "64"   :
                        FXMLDocumentController.CHANNELS = 64;
                        FXMLDocumentController.InitParam[2] = 8;
                        FXMLDocumentController.LabelShiftY = 0;
                        FXMLDocumentController.full_window = 2;
                        break;
                }
                    
                YAxis.setUpperBound(UpperBoundY*CHANNELS*2);
                YAxis.setLowerBound(-UpperBoundY*CHANNELS*2);
                
                FXMLDocumentController.offset = (2*UpperBoundY);
                for(int i=0; i<FXMLDocumentController.CHANNELS;i++){
                    LabelChList.get(i).setVisible(true);
                    LabelChList.get(i).setTranslateY(LabelShiftY*(i+1));
                }
                for(int i=FXMLDocumentController.CHANNELS; i<FXMLDocumentController.MAXCHANNELS;i++){
                    LabelChList.get(i).setVisible(false);
                }
            }
        });
        
        ADSModelBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "ADS1198"   :    
                        SampleRateBox.setItems(FXCollections.observableArrayList("125 SPS","250 SPS","500 SPS","1 KSPS","2 KSPS","4 KSPS", "8 KSPS"));
                        FXMLDocumentController.ADSType = 1;
                        FXMLDocumentController.bytesPerChannel = 2;
                        FXMLDocumentController.InitParam[0] = 0x03;
                        FXMLDocumentController.SR = 1000;
                        SampleRateBox.setValue("1 KSPS");
                        
                        break;
                    case "ADS131E08" :
                        SampleRateBox.setItems(FXCollections.observableArrayList("1 KSPS", "2 KSPS", "4 KSPS", "8 KSPS"));
                        ADSType = 2;
                        bytesPerChannel = 3;
                        FXMLDocumentController.InitParam[0] = 0x06;
                        FXMLDocumentController.SR = 1000;
                        SampleRateBox.setValue("1 KSPS");
                        break;
                    case "ADS1298"   :    
                        SampleRateBox.setItems(FXCollections.observableArrayList("500 SPS","1 KSPS","2 KSPS","4 KSPS","8 KSPS","16 KSPS", "32 KSPS"));
                        ADSType = 3;
                        bytesPerChannel = 3;
                        FXMLDocumentController.InitParam[0] = 0x05;
                        FXMLDocumentController.SR = 1000;
                        SampleRateBox.setValue("1 KSPS");
                        break;
                }
            }
        });
        
        SampleRateBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "125 SPS"   :    
                        XScaleBox.setItems(FXCollections.observableArrayList("4s","10s","20s","40s"));
                        FXMLDocumentController.InitParam[0] = 0x06;
                        FXMLDocumentController.SR = 125;
                        break;
                    case "250 SPS"   :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        FXMLDocumentController.InitParam[0] = 0x05;
                        FXMLDocumentController.SR = 250;
                        break;
                    case "500 SPS"   :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        if(ADSType == 1)
                            FXMLDocumentController.InitParam[0] = 0x04;
                        else
                            FXMLDocumentController.InitParam[0] = 0x06;
                        
                        FXMLDocumentController.SR = 500;
                        break;
                    case "1 KSPS"  :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        if(ADSType == 1)
                            FXMLDocumentController.InitParam[0] = 0x03;
                        else if(ADSType == 2)
                            FXMLDocumentController.InitParam[0] = 0x06;
                        else
                            FXMLDocumentController.InitParam[0] = 0x05;
                        
                        FXMLDocumentController.SR = 1000;
                        break;
                    case "2 KSPS"  :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        if(ADSType == 1)
                            FXMLDocumentController.InitParam[0] = 0x02;
                        else if (ADSType == 2)
                            FXMLDocumentController.InitParam[0] = 0x05;
                        else
                            FXMLDocumentController.InitParam[0] = 0x04;
                        FXMLDocumentController.SR = 2000;
                        break;
                    case "4 KSPS"  :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        if(ADSType == 1)
                            FXMLDocumentController.InitParam[0] = 0x01;
                        else if(ADSType == 2)
                            FXMLDocumentController.InitParam[0] = 0x04;
                        else
                            FXMLDocumentController.InitParam[0] = 0x03;
                        FXMLDocumentController.SR = 4000;
                        break;
                    case "8 KSPS"    :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        if(ADSType == 1)
                            FXMLDocumentController.InitParam[0] = 0x00;
                        else if (ADSType == 2)
                            FXMLDocumentController.InitParam[0] = 0x03;
                        else
                            FXMLDocumentController.InitParam[0] = 0x02;
                        FXMLDocumentController.SR = 8000;
                        break;
                    case "16 KSPS"    :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        if(ADSType == 3)
                            FXMLDocumentController.InitParam[0] = 0x01;
                        FXMLDocumentController.SR = 16000;
                        break;
                    case "32 KSPS"    :
                        XScaleBox.setItems(FXCollections.observableArrayList("2s","4s","10s","20s","40s"));
                        if(ADSType == 3)
                            FXMLDocumentController.InitParam[0] = 0x00;
                        FXMLDocumentController.SR = 32000;
                        break;
                }
                ScaleCount = SR*scaleX/MAX_DATA_POINTS;
                System.out.println("ScaleCount = "+ ScaleCount+ "Sample Rate "+ InitParam[0]);
            }
        });
        
        ModeADSBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "NORMAL"   :    
                        FXMLDocumentController.InitParam[1] = 0x00;
                        break;
                    case "TEST SIGNAL"   :
                        FXMLDocumentController.InitParam[1] = 0x05;
                        break;
                   
                }
            }
        });
        
        GainBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "1"   :    
                        FXMLDocumentController.InitParam[4] = 0x10;
                        FXMLDocumentController.AVAC = 1;
                        break;
                    case "2"   :
                        FXMLDocumentController.InitParam[4] = 0x20;
                        FXMLDocumentController.AVAC = 2;
                        break;
                    case "3"   :    
                        FXMLDocumentController.InitParam[4] = 0x30;
                        FXMLDocumentController.AVAC = 3;
                        break;
                    case "4"   :
                        FXMLDocumentController.InitParam[4] = 0x40;
                        FXMLDocumentController.AVAC = 4;
                        break;
                    case "6"   :
                        FXMLDocumentController.InitParam[4] = 0x00;
                        FXMLDocumentController.AVAC = 6;
                        break;
                    case "8"   :    
                        FXMLDocumentController.InitParam[4] = 0x50;
                        FXMLDocumentController.AVAC = 8;
                        break;
                    case "12"   :
                        FXMLDocumentController.InitParam[4] = 0x60;
                        FXMLDocumentController.AVAC = 12;
                        break;   
                }
            }
        });
        
        ChipSelectBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "4"   :    
                        FXMLDocumentController.InitParam[3] = 4;
                        break;
                    case "10"   :
                        FXMLDocumentController.InitParam[3] = 10;
                        break;
                    case "52"   :
                        FXMLDocumentController.InitParam[3] = 52;
                        break; 
                }
            }
        });
        
        
        
        
        YScaleBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "10uV"   :   
                        UpperBoundY = 10;
                        scaleY = 1e-6;
                    break;
                    case "20uV"   :
                        UpperBoundY = 20;
                        scaleY = 1e-6;
                        break;
                    case "50uV"   :
                        UpperBoundY = 50;
                        scaleY = 1e-6;
                        break;
                    case "100uV"  :
                        UpperBoundY = 100;
                        scaleY = 1e-6;
                        break;
                    case "200uV"  :
                        UpperBoundY = 200;
                        scaleY = 1e-6;
                        break;
                    case "500uV"  :
                        UpperBoundY = 500;
                        scaleY = 1e-6;
                        break;
                    case "1mV"    :
                        UpperBoundY = 1;
                        scaleY = 1e-3;
                        break;
                    case "10mV"   :
                        UpperBoundY = 10;
                        scaleY = 1e-3;    
                        break;
                    case "100mV"  :
                        UpperBoundY = 100;
                        scaleY = 1e-3;
                        break;
                }
                
                YAxis.setUpperBound(UpperBoundY*CHANNELS*2);
                YAxis.setLowerBound(-UpperBoundY*CHANNELS*2);
                offset = (2*UpperBoundY);
            }
        });
        
        XScaleBox.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "2s"   :
                        SerialCOM.resetScaleCount();
                        scaleX = 2;
                        break;
                    case "4s"   :
                        SerialCOM.resetScaleCount();
                        scaleX = 4;
                        break;
                    case "10s"   :
                        SerialCOM.resetScaleCount();
                        scaleX = 10;
                        break;
                    case "20s"  :
                        SerialCOM.resetScaleCount();
                        scaleX = 20;
                        break;
                    case "40s"  :
                        SerialCOM.resetScaleCount();
                        scaleX = 40;
                        break;
                }
                ScaleCount = SR*scaleX/MAX_DATA_POINTS;
                System.out.println("ScaleCount = "+ ScaleCount);
            }
        });
        
        FilterChoice.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue != null) {
                switch(newValue){
                    case "NoFilter"   :
                        SerialCOM.setFilter(0,SR);
                        break;
                    case "0.5-65Hz"   :
                        SerialCOM.setFilter(1,SR);
                        break;
                    case "0.5-40Hz"   :
                        SerialCOM.setFilter(2,SR);
                        break;
                    case "0.5-20Hz"   :
                        SerialCOM.setFilter(3,SR);
                        break;
                    case "2-65Hz"  :
                        SerialCOM.setFilter(4,SR);
                        break;
                    case "2-40Hz"  :
                        SerialCOM.setFilter(5,SR);
                        break;
                    case "2-20Hz"  :
                        SerialCOM.setFilter(6,SR);
                        break;
                }
            }
        });  
    }
    
    //Menu Item Connection
    @FXML
    private void handleMenuItemConnect(ActionEvent event) {
     
        if(Connect.isSelected()){     
            
            if(SerialCOM.StartADS(SelectedPort)){
                SerialCOM.initialize(OutputFileText.getText(), SelectedPort, CalibrationFileText.getText());
                Connect.setSelected(true);
                ConnectButton.setText("Disconnect");
                ChannelsBox.setDisable(true);
                SampleRateBox.setDisable(true);
                ModeADSBox.setDisable(true);
                ChipSelectBox.setDisable(true);
                ADSModelBox.setDisable(true);
                GainBox.setDisable(true);
            }
            else{
                Alert alert = new Alert(AlertType.ERROR);
                alert.setTitle("COM Port Error");
                alert.setHeaderText("The selected COM port is not connected to Creamino");
                alert.setContentText("Look for the connection and try again!");
                alert.showAndWait();
            } 
        }
        else{
            SerialCOM.closePort();
            Connect.setSelected(false);
            ConnectButton.setText("Connect");
            ChannelsBox.setDisable(false);
            SampleRateBox.setDisable(false);
            ModeADSBox.setDisable(false);
            ChipSelectBox.setDisable(false);
            ADSModelBox.setDisable(false);
            GainBox.setDisable(false);
        }
    }
    
    // Button Connection
    @FXML
    private void handleButtonConnect(ActionEvent event) {
            if(!Connect.isSelected()){                        
            if(SerialCOM.StartADS(SelectedPort)){
                SerialCOM.initialize(OutputFileText.getText(), SelectedPort, CalibrationFileText.getText());
                Connect.setSelected(true);
                ConnectButton.setText("Disconnect");
                ChannelsBox.setDisable(true);
                SampleRateBox.setDisable(true);
                ModeADSBox.setDisable(true);
                ChipSelectBox.setDisable(true);
                GainBox.setDisable(true);
                ADSModelBox.setDisable(true);
                }
            else{
                Alert alert = new Alert(AlertType.ERROR);
                alert.setTitle("COM Port Error");
                alert.setHeaderText("The selected COM port is not connected to Creamino");
                alert.setContentText("Look for the connection and try again!");
                alert.showAndWait();
            }
        }
        else{
            SerialCOM.closePort();
            Connect.setSelected(false);
            ConnectButton.setText("Connect");
            ChannelsBox.setDisable(false);
            SampleRateBox.setDisable(false);
            ModeADSBox.setDisable(false);
            ChipSelectBox.setDisable(false);
            ADSModelBox.setDisable(false);
            GainBox.setDisable(false);
        }
    }
    //Notch Filter
    @FXML
    private void handleNotchFilterAction(ActionEvent event) {
        if(NotchFilter.isSelected())
            SerialCOM.SetNotch(SR);
        else
            SerialCOM.ResetNotch();
    }
    

    // Menu Item Close
    @FXML
    private void handleMenuItemClose(ActionEvent event) {
        Platform.exit();
    }
    
    
    @FXML
    private void handleButtonAction(ActionEvent event) {
        
    }
    
    
    @FXML
    private void handleMenuItemChannels(ActionEvent event) {
        

        if(Channels1_8.isSelected())
            for(int i=0; i<8; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=0; i<8; i++)
            SelectedChannelList.get(i).setSelected(false); 
        
        if(Channels9_16.isSelected())
            for(int i=8; i<16; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=8; i<16; i++)
            SelectedChannelList.get(i).setSelected(false);
        
        if(Channels17_24.isSelected())
            for(int i=16; i<24; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=16; i<24; i++)
            SelectedChannelList.get(i).setSelected(false);
        
        if(Channels25_32.isSelected())
            for(int i=24; i<32; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=24; i<32; i++)
            SelectedChannelList.get(i).setSelected(false);
        
        if(Channels33_40.isSelected())
            for(int i=32; i<40; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=32; i<40; i++)
            SelectedChannelList.get(i).setSelected(false);
        
        if(Channels41_48.isSelected())
            for(int i=40; i<48; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=40; i<48; i++)
            SelectedChannelList.get(i).setSelected(false);
        
        if(Channels49_56.isSelected())
            for(int i=48; i<56; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=48; i<56; i++)
            SelectedChannelList.get(i).setSelected(false);
        
        if(Channels57_64.isSelected())
            for(int i=56; i<64; i++)
            SelectedChannelList.get(i).setSelected(true);
        else
            for(int i=56; i<64; i++)
            SelectedChannelList.get(i).setSelected(false);
        
        offset = (2*UpperBoundY);

        
    }
    
    
   @FXML
    private void handleMenuItemReference(ActionEvent event) throws IOException {
        stageReference.show();
    }
   
   @FXML
    private void handleMenuItemCalibration(ActionEvent event) throws IOException {
        stageCalibration.show();
    }
    
    
    @FXML
    private void handleMenuItemSaveLabels(ActionEvent event) throws IOException {
        for(int i=0; i<FXMLDocumentController.MAXCHANNELS;i++){
                    CHLabel[i]=LabelChList.get(i).getText();
                } 
                
        try {
        PrintWriter writer = new PrintWriter(new File("Label.txt"));
            for(int i=0; i<FXMLDocumentController.MAXCHANNELS;i++)
                //use this if your array has String
                writer.write(CHLabel[i]+" "); //Its String so you dont have to use String.valueOf(something(int,double,...)
                //use this if your array has (int,double..)
                // writer.write(String.valueOf(a[i][j])+" "); //Here you parse the int from array to String. 
        
            writer.println(); //leave one line 
            writer.flush();  //flush the writer
            writer.close();  //close the writer      


        } catch (FileNotFoundException e) {      
            e.printStackTrace();
        }              
    }
    
    @FXML
    private void handleMenuItemLoadLabels(ActionEvent event) throws IOException {
        
        try {
            Scanner scan = new Scanner(new File("Label.txt"));

            //Retrieve
            for(int i=0; i<FXMLDocumentController.MAXCHANNELS;i++){
                CHLabel[i]=scan.next();
                LabelChList.get(i).setText(CHLabel[i]);
            }
            scan.close(); //close the resource file you opened
                       
        } catch (FileNotFoundException e){  e.printStackTrace(); }    
                
             
    }
    
    @FXML
    private void handleButtonSearch (ActionEvent event) throws IOException {
        Ports = SerialCOM.CheckPorts();
            //for (String portName : Ports)
                COMBox.setItems(FXCollections.observableArrayList(Ports));
    }
            
    @FXML
    private void handleMenuItemP100 (ActionEvent event) throws IOException {        
        
        MAX_DATA_POINTS = 100;
        
        SelectedPoints.get(1).setSelected(false);
        SelectedPoints.get(2).setSelected(false);
        SelectedPoints.get(3).setSelected(false);
        SelectedPoints.get(4).setSelected(false);
        
        XAxis.setUpperBound(MAX_DATA_POINTS);
            
        ScaleCount = SR*scaleX/MAX_DATA_POINTS;
        System.out.println("ScaleCount = "+ ScaleCount);
        
        
    }
    
    @FXML
    private void handleMenuItemP200 (ActionEvent event) throws IOException {
        
        MAX_DATA_POINTS = 200;
        
        SelectedPoints.get(0).setSelected(false);
        SelectedPoints.get(2).setSelected(false);
        SelectedPoints.get(3).setSelected(false);
        SelectedPoints.get(4).setSelected(false);
        
        XAxis.setUpperBound(MAX_DATA_POINTS);
         
        ScaleCount = SR*scaleX/MAX_DATA_POINTS;
        System.out.println("ScaleCount = "+ ScaleCount);
        
    }
    
    @FXML
    private void handleMenuItemP300 (ActionEvent event) throws IOException {
        
        MAX_DATA_POINTS = 300;
        
        SelectedPoints.get(0).setSelected(false);
        SelectedPoints.get(1).setSelected(false);
        SelectedPoints.get(3).setSelected(false);
        SelectedPoints.get(4).setSelected(false);
        
        XAxis.setUpperBound(MAX_DATA_POINTS);
         
        ScaleCount = SR*scaleX/MAX_DATA_POINTS;
        System.out.println("ScaleCount = "+ ScaleCount);
         
    }
    
    @FXML
    private void handleMenuItemP400 (ActionEvent event) throws IOException {
        
        MAX_DATA_POINTS = 400;
        
        SelectedPoints.get(0).setSelected(false);
        SelectedPoints.get(1).setSelected(false);
        SelectedPoints.get(2).setSelected(false);
        SelectedPoints.get(4).setSelected(false);
        
        XAxis.setUpperBound(MAX_DATA_POINTS);
    
        ScaleCount = SR*scaleX/MAX_DATA_POINTS;
        System.out.println("ScaleCount = "+ ScaleCount);
        
         
    }
    
    @FXML
    private void handleMenuItemP500 (ActionEvent event) throws IOException {
        
        MAX_DATA_POINTS = 500;
        
        SelectedPoints.get(0).setSelected(false);
        SelectedPoints.get(1).setSelected(false);
        SelectedPoints.get(2).setSelected(false);
        SelectedPoints.get(3).setSelected(false);

        XAxis.setUpperBound(MAX_DATA_POINTS);
         
        ScaleCount = SR*scaleX/MAX_DATA_POINTS;
        System.out.println("ScaleCount = "+ ScaleCount);
        
    }
    
    @FXML
    private void handleMenuItemfullsize (ActionEvent event) throws IOException {
        if(full_size.isSelected())
            full_window = 1;
        else
            full_window = 2;
    }
 
    
    private void initReference() throws IOException{
        
        fxmlLoader = new FXMLLoader(getClass().getResource("Reference.fxml"));
        fxmlLoader.setControllerFactory(new Callback<Class<?>, Object>() {
            @Override
            public Object call(Class<?> controllerClass) {
                if (controllerClass == ReferenceController.class) {
                    ReferenceCtrl = new ReferenceController();
                    CH = ReferenceCtrl.getIndex();
                    return ReferenceCtrl ;
                } else {
                    try {
                        return controllerClass.newInstance();
                    } catch (Exception exc) {
                        throw new RuntimeException(exc); // just bail
                    }
                }
            }
        });
        
        rootReference = (Parent) fxmlLoader.load();
        stageReference = new Stage();
        stageReference.setTitle("Set Reference");
        stageReference.setScene(new Scene(rootReference));  
        
    }
    
    private void initCalibration() throws IOException{
        
        fxmlLoader = new FXMLLoader(getClass().getResource("Calibration.fxml"));
        fxmlLoader.setControllerFactory(new Callback<Class<?>, Object>() {
            @Override
            public Object call(Class<?> controllerClass) {
                if (controllerClass == CalibrationController.class) {
                    CalibrationCtrl = new CalibrationController();
                    return CalibrationCtrl ;
                } else {
                    try {
                        return controllerClass.newInstance();
                    } catch (Exception exc) {
                        throw new RuntimeException(exc); // just bail
                    }
                }
            }
        });
        
        rootCalibration = (Parent) fxmlLoader.load();
        stageCalibration = new Stage();
        stageCalibration.setTitle("Set Calibration");
        stageCalibration.setScene(new Scene(rootCalibration));  
       
    }
    
}