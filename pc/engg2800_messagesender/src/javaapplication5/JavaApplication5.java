/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication5;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
/**
 *
 * @author Michael
 */
public class JavaApplication5 extends JFrame {
    
    //literals for testing here
    
    private static final int RATE = 100;
    private static final int WORDLENGTH = 5;
    private static final int CLEARWEATHER = 0;
    private static final int CLOUDYWEATHER = 1;
    private static final int RAINYWEATHER = 2;
    private static final int ALARMHOURTEXT = 0;
    private static final int ALARMMINUTETEXT = 1;
    private static final int CLOCKHOURTEXT = 2;
    private static final int CLOCKMINUTETEXT = 3;
    private static final int TODAYFORECAST = 0;
    private static final int TOMMOROWFORECAST = 2;
    
    private static final 
            String WEATHERURL = "http://api.wunderground.com/api/ee6756280a23f6e9/forecast/q/YBBN.xml";
    
    private static final Color HIGHCOLOR = Color.BLACK;
    private static final Color LOWCOLOR = Color.WHITE;
    
    
    
    
    public static void main(String[] args) throws InterruptedException {
    new JavaApplication5();
  }

    /**
     * @param args the command line arguments
     */
    public JavaApplication5() throws InterruptedException {
        super("Message Sender");
        WindowUtilities.setNativeLookAndFeel();
        addWindowListener(new ExitListener());
        Container content = getContentPane();
        content.setBackground(Color.lightGray);
        JPanel controlArea = new JPanel(new GridLayout(3, 1));
        String[] labels = { "Alarm Hour", "Alarm Minute", 
            "Clock Hour", "Clock Minute" };
        char[] mnemonics = { 'F', 'M', 'L', 'A' };
        int[] widths = { 3, 3, 3, 3 };
        String[] descs = { "addlater", "add later", "add later", "add later" };

        final TextForm form = new TextForm(labels, mnemonics, widths, descs);
        controlArea.add(form);
        JCheckBox weatherCheck = new JCheckBox("Set Weather");
        JCheckBox alarmCheck = new JCheckBox("Set Alarm");
        JCheckBox timeCheck = new JCheckBox("Set Time");
        controlArea.add(weatherCheck);
        controlArea.add(alarmCheck);
        controlArea.add(timeCheck);
        JButton submit = new JButton("Program Clock");
        controlArea.add(submit);
        content.add(controlArea, BorderLayout.EAST);
        final JPanel drawingArea = new JPanel();
        drawingArea.setPreferredSize(new Dimension(400, 0));
        drawingArea.setBorder(BorderFactory.createLineBorder (Color.blue, 2));
        drawingArea.setBackground(Color.white);
        content.add(drawingArea, BorderLayout.WEST);
        pack();
        setVisible(true);
        final ArrayList<Integer> list = new ArrayList();
        submit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                ArrayList<Integer> temporaryList = new ArrayList();
                temporaryList.addAll(form.getBinary(ALARMHOURTEXT, WORDLENGTH));
                temporaryList.addAll(form.getBinary(ALARMMINUTETEXT, WORDLENGTH));
                temporaryList.addAll(form.getBinary(CLOCKHOURTEXT, WORDLENGTH));
                temporaryList.addAll(form.getBinary(CLOCKMINUTETEXT, WORDLENGTH));
                list.addAll(temporaryList);
                System.out.println(list.toString());
            }
        });
        while(true){
            while(list.isEmpty());
            int forecast = getForecast();
            list.addAll(addFlags(weatherCheck.isSelected(), alarmCheck.isSelected()
                , timeCheck.isSelected(), forecast));
            changeColor(list, RATE, drawingArea);
            list.removeAll(list);
            JOptionPane.showMessageDialog(null, "Programmed!");
        }
    }
    
    public void changeColor(ArrayList<Integer> message, int rate, JPanel panel) {
        for(int i: message) {
            if(i == 1) {
                panel.setBackground(HIGHCOLOR);
            } else {
               panel.setBackground(LOWCOLOR);
            }
            try {
                Thread.sleep(rate);
            } catch (InterruptedException ex) {
                Logger.getLogger(JavaApplication5.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
    public ArrayList<Integer> addFlags(Boolean setWeather, 
            Boolean setAlarm, Boolean setTime, int forecast) {
            ArrayList<Integer> list = new ArrayList();
            if(setWeather) {
                list.add(1);
            } else {
                list.add(0);
            }
            if(setAlarm) {
                list.add(1);
            } else {
                list.add(0);
            }
            if(setTime) {
                list.add(1);
            } else {
                list.add(0);
            }
            switch (forecast) {
                case CLEARWEATHER: list.add(0);
                        list.add(0);
                        break;
                case CLOUDYWEATHER: list.add(0);
                        list.add(1);
                        break;
                case RAINYWEATHER: list.add(1);
                        list.add(0);
                        break;
            }
            System.out.println(list.toString());
            return list;
    }
    
    public int getForecast() {
        String forecast;
        try {
            URL forecastUrl = new URL(WEATHERURL);
            InputStream openStream = forecastUrl.openStream();
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.parse(openStream);
            NodeList formattedForecast = doc.getElementsByTagName("icon");

            Element formattedForecastElement = 
                    (Element) formattedForecast.item(TOMMOROWFORECAST);
            forecast = formattedForecastElement.getTextContent();
        } catch (Exception e) {
            forecast = "";
        }
        
        if(forecast.contains("clear")) {
            return CLEARWEATHER;
        } else if(forecast.contains("cloudy")) {
            return CLOUDYWEATHER;
        } else if(forecast.contains("rain")) {
            return RAINYWEATHER;
        } else {
            return CLEARWEATHER;
        }
    }
}
