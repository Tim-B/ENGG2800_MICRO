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
 * @author Michael Banfield 42357982
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
    private static final String IPURL = "http://api.wunderground.com/api/ee6756280a23f6e9/geolookup/q/autoip.xml";
    private static final Color HIGHCOLOR = Color.WHITE;
    private static final Color LOWCOLOR = Color.BLACK;

    /**
     *
     * @param args
     * @throws InterruptedException
     */
    public static void main(String[] args) throws InterruptedException {
        new JavaApplication5();
    }

    /**
     * @throws InterruptedException
     */
    public JavaApplication5() throws InterruptedException {
        super("Message Sender");
        WindowUtilities.setNativeLookAndFeel();
        addWindowListener(new ExitListener());
        Container content = getContentPane();
        content.setBackground(Color.lightGray);
        JPanel controlArea = new JPanel(new GridLayout(3, 1));
        String[] labels = {"Alarm Hour", "Alarm Minute",
            "Clock Hour", "Clock Minute"};
        char[] mnemonics = {'F', 'M', 'L', 'A'};
        int[] widths = {3, 3, 3, 3};
        String[] descs = {"addlater", "add later", "add later", "add later"};
        final TextForm form = new TextForm(labels, mnemonics, widths, descs);
        controlArea.add(form);
        JCheckBox weatherCheck = new JCheckBox("Set Weather");
        JCheckBox alarmCheck = new JCheckBox("Set Alarm");
        JCheckBox activeAlarmCheck = new JCheckBox("Alarm Active");
        controlArea.add(weatherCheck);
        controlArea.add(alarmCheck);
        controlArea.add(activeAlarmCheck);
        JButton submit = new JButton("Program Clock");
        controlArea.add(submit);
        content.add(controlArea, BorderLayout.EAST);
        final JPanel drawingArea = new JPanel();
        drawingArea.setPreferredSize(new Dimension(400, 0));
        drawingArea.setBorder(BorderFactory.createLineBorder(Color.blue, 2));
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
                temporaryList.addAll(form.getBinary(CLOCKHOURTEXT,
                        CLOCKMINUTETEXT, LONGLENGTH));
                temporaryList.addAll(form.getBinary(ALARMHOURTEXT,
                        ALARMMINUTETEXT, LONGLENGTH));
                list.addAll(temporaryList);
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
            list.removeAll(list);
            drawingArea.setBackground(Color.white);
            JOptionPane.showMessageDialog(null, "Programmed!");
        }
    }

    /**
     *
     * @param message
     * @param rate
     * @param panel
     * @param high
     * @param low
     */
    public void changeColor(ArrayList<Integer> message, int rate, JPanel panel, Color high, Color low) {
        for (int i : message) {
            if (i == 1) {
                panel.setBackground(high);

            } else {
                panel.setBackground(low);
            }
            try {
                Thread.sleep(rate);
            } catch (InterruptedException ex) {
                Logger.getLogger(JavaApplication5.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    /**
     *
     * @param setWeather
     * @param setAlarm
     * @param setActive
     * @param forecast
     * @return
     */
    public ArrayList<Integer> addFlags(Boolean setWeather,
            Boolean setAlarm, Boolean setActive, int forecast) {
        ArrayList<Integer> list = new ArrayList();
        if (setAlarm) {
            list.add(1);
        } else {
            list.add(0);
        }
        if (setActive) {
            list.add(1);
        } else {
            list.add(0);
        }
        if (setWeather) {
            list.add(1);
        } else {
            list.add(0);
        }
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
        list.add(0);
        list.add(0);
        list.add(0);
        return list;
    }

    /**
     *
     * @return
     */
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

    /**
     *
     * @return
     */
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
