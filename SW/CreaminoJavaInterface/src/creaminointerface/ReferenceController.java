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

import java.net.URL;
import java.util.List;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.CheckBox;


/**
 *
 * @author Matteo Chiesi - University of Bologna
 */
public class ReferenceController implements Initializable {
    
    public final int[] CH = new int[64];

    @FXML
    public List<CheckBox> SelectedReference;
    
    
    public ReferenceController() {
        
    }
    
    @FXML
    private void handleCheckBoxCh(ActionEvent event){
        for(int i=0;i<64;i++)
            if(SelectedReference.get(i).isSelected())
                CH[i]=1;
            else    
                CH[i]=0;           
                
    }
    
    @FXML
    private void handleResetButton(ActionEvent event){
         for(int i=0;i<64;i++){
             SelectedReference.get(i).setSelected(false);
             CH[i]=0;
         }
    }
    
    
    @Override
    public void initialize(URL location, ResourceBundle resources) {
        for(int i=0;i<64;i++)
            CH[i]=0;
          
    }
    
   public int[] getIndex(){
       return CH;
   } 
    
    
}
