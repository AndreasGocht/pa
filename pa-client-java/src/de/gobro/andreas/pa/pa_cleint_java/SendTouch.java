/*
This file is part of pa (Project Anna).

pa (Project Anna) is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pa (Project Anna) is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pa (Project Anna).  If not, see <http://www.gnu.org/licenses/>.
*/

package de.gobro.andreas.pa.pa_cleint_java;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Point;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.Xml;
import android.view.Menu;
import android.view.View;

import java.io.OutputStream;

import org.xmlpull.v1.XmlSerializer;

import de.gobro.andreas.pa.pa_cleint_java.R;

import android.support.v4.view.MotionEventCompat;
import android.view.Display;
import android.view.MotionEvent;
import android.widget.Toast;

public class SendTouch extends Activity {

	Socket clientSocket;
	OutputStream out;
	XmlSerializer serializer;
	Integer maxPoints;
//	
	Display display;
	Point size;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_send_touch);
		View v = findViewById(R.id.main_view);
		v.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN);
		
		maxPoints = 10;
		
		size = new Point();
		
		DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);

		size.x=metrics.widthPixels;
		size.y=metrics.heightPixels;
		
		serializer = Xml.newSerializer();

		new Thread(){
			public void run(){
				try {
				Intent intent = getIntent();
				//clientSocket = new Socket("192.168.43.245",15000);
				clientSocket = new Socket(intent.getStringExtra(MainActivity.IP_FIELD),15000);
				//TODO checkt, if connection is etabished
				
				out = clientSocket.getOutputStream();
//				xml with
//				http://www.ibm.com/developerworks/opensource/library/x-android/index.html#N102B3
				
				
				 
				serializer.setOutput(out, "UTF-8");

				serializer.startTag("", "pa_smart_gui");
				serializer.startTag("", "init");
					serializer.startTag("", "maxTouch");
						serializer.text(maxPoints.toString());
					serializer.endTag("", "maxTouch");
				serializer.endTag("", "init");
				serializer.flush();
					
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalStateException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				Log.d("net","some error !!!!2");
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				Log.d("net","some conection error");
				e.printStackTrace();
				finish();

			}
				
			}
		}.start();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.send_touch, menu);
		return true;
	}

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
    	final int action = MotionEventCompat.getActionMasked(ev);
    	
    	switch (action) { 
    	case MotionEvent.ACTION_DOWN: {
    		Log.d("down","action_down");
    		break;
    	}
    	case MotionEvent.ACTION_MOVE:{
    		for(Integer i = 0; (i< ev.getPointerCount()) && (i< maxPoints);i++)
    		{
	    		//final Integer pointerIndex = MotionEventCompat.getActionIndex(ev);

	    		Integer id = ev.getPointerId(i);
	    		Log.d("ind","pinterindex" + i.toString());
	    		Log.d("ind","ID" + id.toString());
	    		Log.d("pos","("+ String.valueOf(MotionEventCompat.getX(ev, i)) +";"+String.valueOf(MotionEventCompat.getY(ev, i))+")");
	    		Log.d("test","action_move");
				
	    		try {
					serializer.startTag("", "touch");
						serializer.startTag("", "dx");
							serializer.text(String.valueOf(MotionEventCompat.getX(ev, i)/size.x));
						serializer.endTag("", "dx");
						serializer.startTag("", "dy");
							serializer.text(String.valueOf(MotionEventCompat.getY(ev, i)/size.y));
						serializer.endTag("", "dy");
						serializer.startTag("", "id");
							serializer.text(id.toString());
						serializer.endTag("", "id");
					serializer.endTag("", "touch");
					

				} catch (IllegalArgumentException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IllegalStateException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (RuntimeException e){
					e.printStackTrace();
				}
				
	    		
//	    		try {
//					writer.writeStartElement("touch");
//						writer.writeStartElement("dx");
//							writer.writeCharacters(String.valueOf(MotionEventCompat.getX(ev, i)/size.x));
//						writer.writeEndElement();
//						writer.writeStartElement("dy");
//							writer.writeCharacters(String.valueOf(MotionEventCompat.getY(ev, i)/size.y));
//						writer.writeEndElement();
//						writer.writeStartElement("id");
//							writer.writeCharacters(id.toString());
//						writer.writeEndElement();						
//					writer.writeEndElement();
//				} catch (XMLStreamException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
	    		
    		}
    		
    		//write the stuff to the network
    		try {
				serializer.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				Toast.makeText(this,e.getMessage(),Toast.LENGTH_SHORT).show();
				finish();

			}
    		break;
    	}
    	case MotionEvent.ACTION_UP:{
    		Log.d("test","action_up");
    		break;
    	}
    	case MotionEvent.ACTION_CANCEL:{
    		Log.d("test","action_cancel");
    		break;
    	}
    	case MotionEvent.ACTION_POINTER_DOWN:{
    		Log.d("down","action_p_down");
    		break;
    	}
    	case MotionEvent.ACTION_POINTER_UP: {
    		Log.d("test","action_p_up");
    		break;
    	}
    	}   
    	return true;
    }
}
