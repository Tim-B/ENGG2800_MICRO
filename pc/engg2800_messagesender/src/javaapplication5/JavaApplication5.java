
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
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
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
    private static final int RATE = 160;
    private static final int WORDLENGTH = 8;
    private static final int LONGLENGTH = WORDLENGTH * 2;
    private static final int CLEARWEATHER = 0;
    private static final int CLOUDYWEATHER = 1;
    private static final int RAINYWEATHER = 2;
    private static final int ALARMHOURTEXT = 0;
    private static final int ALARMMINUTETEXT = 1;
    private static final int CLOCKHOURTEXT = 2;
    private static final int CLOCKMINUTETEXT = 3;
    private static final int TODAYFORECAST = 0;
    private static final int TOMMOROWFORECAST = 2;
    private static final String IPURL 
            = "http://api.wunderground.com/api/ee6756280a23f6e9/geolookup/q/autoip.xml";
    private static final Color HIGHCOLOR = Color.WHITE;
    private static final Color LOWCOLOR = Color.BLACK;

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
        GridBagConstraints c = new GridBagConstraints();
        JPanel controlArea = new JPanel(new GridBagLayout());
        controlArea.setBorder(new EmptyBorder(10, 10, 10, 10) );
        this.setResizable(false);
        String[] labels = {"Alarm Hour", "Alarm Minute",
            "Clock Hour", "Clock Minute"};
        char[] mnemonics = {'F', 'M', 'L', 'A'};
        int[] widths = {3, 3, 3, 3};
        String[] descs = {"Must be in 24 hour time, between 0 and 23", 
            "Must be in 24 hour time, between 0 and 59", 
            "Must be in 24 hour time, between 0 and 23", 
            "Must be in 24 hour time, between 0 and 59"};
        
        final TextForm form = new TextForm(labels, mnemonics, widths, descs);
        c.fill = GridBagConstraints.CENTER;
        c.gridheight = 1;
        c.gridwidth = 1;
        c.gridx = 0;
        c.gridy = 0;
        c.fill = GridBagConstraints.HORIZONTAL;
        
        JLabel timeLabel = new JLabel("Time settings");
        timeLabel.setFont(new Font("Sans Serif", Font.BOLD, 12));
        timeLabel.setBorder(BorderFactory.createMatteBorder(0, 0, 1, 0,Color.BLACK));
        controlArea.add(timeLabel, c);
        
        c.gridx = 0;
        c.gridy = 1;  
        c.gridwidth = 1;
        c.gridheight = 1;
        controlArea.add(form, c);
        
        c.gridx = 0;
         
        c.gridwidth = 1;
        c.gridheight = 1;
        JCheckBox weatherCheck = new JCheckBox("Set Weather");
        JCheckBox alarmCheck = new JCheckBox("Set Alarm");
        JCheckBox activeAlarmCheck = new JCheckBox("Alarm Active");
        
        c.gridy = 2; 
        JLabel settingsLabel = new JLabel("Other settings");
        settingsLabel.setFont(new Font("Sans Serif", Font.BOLD, 12));
        settingsLabel.setBorder(BorderFactory.createMatteBorder(0, 0, 1, 0,Color.BLACK));
        controlArea.add(settingsLabel, c);
        
        c.gridy = 3; 
        controlArea.add(weatherCheck, c);
        c.gridy = 4;
        controlArea.add(alarmCheck, c);
        c.gridy = 5;
        controlArea.add(activeAlarmCheck, c);
        
        JButton submit = new JButton("Program Clock");
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 2;
        c.gridx = 0;
        c.gridy = 6;
        controlArea.add(submit, c);
        content.add(controlArea, BorderLayout.EAST);

        final JPanel drawingArea = new JPanel();
        drawingArea.setPreferredSize(new Dimension(400, 400));
        drawingArea.setBorder(BorderFactory.createLineBorder(Color.black, 10));
        drawingArea.setBackground(Color.white);

        content.add(drawingArea, BorderLayout.WEST);
        pack();
        setVisible(true);
        final ArrayList<Integer> list = new ArrayList();
        final ArrayList<Integer> startSequence = new ArrayList();
        startSequence.add(0);
        startSequence.add(1);
        startSequence.add(1);
        startSequence.add(0);
        startSequence.add(1);
        startSequence.add(0);
        startSequence.add(1);
        startSequence.add(1);
        submit.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent e) {
                ArrayList<Integer> temporaryList = new ArrayList();
                temporaryList.addAll(form.getBinary(CLOCKHOURTEXT, CLOCKMINUTETEXT, LONGLENGTH));
                temporaryList.addAll(form.getBinary(ALARMHOURTEXT, ALARMMINUTETEXT, LONGLENGTH));
                list.addAll(temporaryList);
                System.out.println(list.toString());
            }
        });
        while (true) {
            while (list.isEmpty()) {
                Thread.sleep(0);
            }

            int forecast = getForecast();
            list.addAll(addFlags(weatherCheck.isSelected(), alarmCheck.isSelected(), activeAlarmCheck.isSelected(), forecast));
            long profileTime = System.currentTimeMillis();
            changeColor(startSequence, RATE, drawingArea, HIGHCOLOR, LOWCOLOR);
            changeColor(list, RATE, drawingArea, HIGHCOLOR, LOWCOLOR);
            changeColor(list, RATE, drawingArea, LOWCOLOR, HIGHCOLOR);
            changeColor(startSequence, RATE, drawingArea, LOWCOLOR, HIGHCOLOR);
            long endTime = System.currentTimeMillis() - profileTime;
            System.out.println(endTime);
            list.removeAll(list);
            drawingArea.setBackground(Color.white);
            JOptionPane.showMessageDialog(null, "Programmed!");
            System.out.print("\n");
        }
    }

    public void changeColor(ArrayList<Integer> message, int rate, JPanel panel, Color high, Color low) {
        for (int i : message) {
            if (i == 1) {
                panel.setBackground(high);
                // System.out.print("1");

            } else {
                panel.setBackground(low);
                // System.out.print("0");
            }
            try {
                Thread.sleep(rate);
            } catch (InterruptedException ex) {
                Logger.getLogger(JavaApplication5.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        // System.out.print("\n");
    }

    public ArrayList<Integer> addFlags(Boolean setWeather,
            Boolean setAlarm, Boolean setActive, int forecast) {
        ArrayList<Integer> list = new ArrayList();
        //Alarm changed
        if (setAlarm) {
            list.add(1);
        } else {
            list.add(0);
        }
        //Alarm Active
        if (setActive) {
            list.add(1);
        } else {
            list.add(0);
        }
        //Weather Changed
        if (setWeather) {
            list.add(1);
        } else {
            list.add(0);
        }
        //Weather bit 1 and 2
        switch (forecast) {
            case CLEARWEATHER:
                list.add(0);
                list.add(0);
                break;
            case CLOUDYWEATHER:
                list.add(0);
                list.add(1);
                break;
            case RAINYWEATHER:
                list.add(1);
                list.add(0);
                break;
        }
        //Three 0's
        list.add(0);
        list.add(0);
        list.add(0);


        System.out.println(list.toString());
        return list;
    }
    
    public String getWeatherStation() {

         String station;
         try {
             URL stationUrl = new URL(IPURL);
             InputStream openStream = stationUrl.openStream();
             DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
             DocumentBuilder db = dbf.newDocumentBuilder();
             Document doc = db.parse(openStream);
             NodeList formattedStation = doc.getElementsByTagName("icao");
            Element formattedStationElement =
                     (Element) formattedStation.item(0);
             station = formattedStationElement.getTextContent();
         } catch (Exception e) {
             station = "";
         }
         return station;
       

     }

         public int getForecast() {
         String forecast;
         String station = getWeatherStation();
        String weatherUrl =
                "http://api.wunderground.com/api/ee6756280a23f6e9/forecast/q/"
                 + station + ".xml";
         try {
             URL forecastUrl = new URL(weatherUrl);
             InputStream openStream = forecastUrl.openStream();
             DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
             DocumentBuilder db = dbf.newDocumentBuilder();
             Document doc = db.parse(openStream);
             NodeList formattedForecast = doc.getElementsByTagName("icon");
            Element formattedForecastElement =
                    (Element) formattedForecast.item(TODAYFORECAST);
             forecast = formattedForecastElement.getTextContent();
         } catch (Exception e) {
             forecast = "";
         }


        if (forecast.contains("clear")) {
             return CLEARWEATHER;

        } else if (forecast.contains("cloudy")) {
             return CLOUDYWEATHER;
        } else if (forecast.contains("rain")) {
             return RAINYWEATHER;
         } else {
             return CLEARWEATHER;
         }
     }
}
