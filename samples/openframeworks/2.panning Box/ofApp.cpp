#include "ofApp.h"

#define BUFFER_SIZE 512
#define SAMPLING_RATE 44100


//--------------------------------------------------------------
void ofApp::setup(){
  m_tempOutputBuffer = new float*[2];
  for (int ch = 0; ch < 2; ++ch) {
    m_tempOutputBuffer[ch] = new float[BUFFER_SIZE];
  }
  // create and start an audio context. it won't be pulled until soundstream is started
  mAudioContext = new AudioContext(SAMPLING_RATE, 2, BUFFER_SIZE, true);
  mAudioContext->start();
  
  // start audio
  ofSoundStreamSetup(2, 0, this, SAMPLING_RATE, BUFFER_SIZE, 4);
  //ofSoundStreamStart();
  
  // create a buffer from file. Note that this throws std::runtime_error
  // in case of errors
  mSynth.mNoteBuffer = mAudioContext->createInMemoryBuffer(ofToDataPath("sounds/SynthNote.aif"));
  mSynth.rvb = new ReverbNode(mAudioContext);
  mSynth.init(mAudioContext);
  m_Box.set(50);
  m_BoxX = (float)ofGetWidth() / 2;
  m_BoxY = (float)ofGetHeight() / 2;
  m_BoxZ = 0;
  m_moveLeft = true;
  m_moveUp = true;
  i = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
  if(m_moveLeft){
    --m_BoxX;
  }else{
    ++m_BoxX;
  }
  if(m_moveUp){
    --m_BoxY;
  }else{
    ++m_BoxY;
  }
  if(m_BoxX <= 0){
    m_moveLeft = false;
  }
  if(m_BoxX >= ofGetWidth()){
    m_moveLeft = true;
  }
  if(m_BoxY <= 0){
    m_moveUp = false;
  }
  if(m_BoxY >= ofGetHeight()){
    m_moveUp = true;
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  m_Box.setPosition(m_BoxX, m_BoxY, m_BoxZ);
  m_Box.draw();
  
  if(i == 10){
    i = 0;
    float nx = (float)m_BoxX / (float)ofGetWidth();
    float ny = (float)m_BoxY / (float)ofGetHeight();
    nx = (nx * 2) - 1;
    ny = (ny * 2) - 1;
    // the playback speed of the sample is between 0.5 and 2
    float playbackSpeed = ofMap((float)m_BoxX / (float)ofGetWidth(), .0F, 1.F, 0.5, 2);
    // the filter frequency 0.1 and 0.5
    float filterFrequency = ofMap((float)(ofGetHeight() - m_BoxY) / (float)ofGetHeight(), .0F, 1.F, 20.F, 20000.F);
    
    // BAAM :)
    mSynth.playNote(mAudioContext, playbackSpeed, filterFrequency,nx,ny,m_BoxX);
  }
  ++i;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  float nx = (float)m_BoxX / (float)ofGetWidth();
  float ny = (float)m_BoxY / (float)ofGetHeight();
  nx = (nx * 2) - 1;
  ny = (ny * 2) - 1;
  // the playback speed of the sample is between 0.5 and 2
  float playbackSpeed = ofMap((float)x / (float)ofGetWidth(), .0F, 1.F, 0.5, 2);
  // the filter frequency 0.1 and 0.5
  float filterFrequency = ofMap((float)(ofGetHeight() - y) / (float)ofGetHeight(), .0F, 1.F, 20.F, 20000.F);
  
  float pan = ofMap((float)x / (float)ofGetWidth(), .0F, 1.F, -1.F, 1.F);

  // BAAM :)
  std::cout << "x " << nx << " y " << ny << std::endl;
  mSynth.playNote(mAudioContext, playbackSpeed, filterFrequency,nx,ny,0);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
  mAudioContext->audioCallback(NULL, 0, m_tempOutputBuffer, nChannels, bufferSize);
  float * outputSample = output;
  for (int i = 0; i < bufferSize; ++i) {
    for (int ch = 0; ch < nChannels; ++ch) {
      *outputSample = m_tempOutputBuffer[ch][i];
      ++outputSample;
    }
  }
}
