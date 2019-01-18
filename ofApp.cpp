#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofEnableDepthTest();
	ofNoFill();
	ofSetLineWidth(2.5);

	this->sound.load("Cold_Killa_Sting.mp3");
	this->sound.play();

	this->n_bands_to_get = 60;
	for (int j = 0; j < this->n_bands_to_get; j++) {
	
		this->fft_smoothed.push_back(0);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSoundUpdate();
	float* val = ofSoundGetSpectrum(this->n_bands_to_get);
	for (int i = 0; i < this->n_bands_to_get; i++) {

		this->fft_smoothed[i] *= 0.98f;
		if (this->fft_smoothed[i] < val[i]) {

			this->fft_smoothed[i] = val[i];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	int radius = 300;
	int deg_span = 10;
	int sound_index = 0;
	for (int deg_start = 0; deg_start < 360; deg_start += deg_span) {

		int deg = deg_start;
		vector<glm::vec3> vertices;
		vector<glm::vec3> sub_vertices;
		int max_height = this->fft_smoothed[sound_index++] * 150 * (sound_index + 1);
		for (int height = 0; height < max_height; height += 10) {

			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), height, radius * sin(deg * DEG_TO_RAD)));
			sub_vertices.push_back(glm::vec3(radius * cos((deg + deg_span) * DEG_TO_RAD), height, radius * sin((deg + deg_span) * DEG_TO_RAD)));
			deg += 5;
		}

		for (int i = sub_vertices.size() - 1; i > -1; i--) {

			vertices.push_back(sub_vertices[i]);
		}

		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);
	}

	this->cam.end();

	if (!this->sound.getIsPlaying()) { this->sound.play(); }
}
//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}