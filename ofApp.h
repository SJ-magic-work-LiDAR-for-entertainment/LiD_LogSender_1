/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxNetwork.h"
#include <ofxGui.h>

/************************************************************
************************************************************/
// #define DEBUG_AS_REALTIME

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class FilesInDir{
private:
	string str_dir_ = "not set yet";
	
	vector<std::ifstream> f_log_;
	vector<string*> file_names_;
	vector<int> frame_id_;
	vector<float> file_fps_;
	
	int file_id_ = 0;
	
	void Align_StringOfData(string& s);
	void DiscardOneLine(std::ifstream& f_log);
	
public:
	~FilesInDir();
	void MakeupFileTable(const string dirname);
	void OpenFiles();
	bool IsEof();
	void FSeekToZero();
	void GetLine(string& str_line);
	void IdNext();
	void IdPrev();
	bool SetId(int id);
	int GetId();
	int GetFrame_Id();
	int GetFileTime();
};

/**************************************************
**************************************************/
struct PointSize{
	float h_;
	float l_;
	
	PointSize(float h, float l)
	: h_(h)
	, l_(l)
	{
	}
};

/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	/********************
	********************/
	ofxPanel gui_;
	ofxGuiGroup Group_Limit;
		ofxToggle gui_b_limit_by_ranging_;
		ofxFloatSlider gui_limit_points_coord_;
		ofxFloatSlider gui_limit_points_to_send_;
		ofxFloatSlider gui_send_per_;
	ofxGuiGroup Group_AutoCutChange;
		ofxToggle gui_b_enable_auto_cut_change;
		ofxToggle gui_b_always_start_from_zero;
	ofxGuiGroup Group_SoundSync;
		ofxToggle gui_b_send_udp_sound_sync_point_size;
	
	/********************
	********************/
	const int kUdpBufSize_ = 100000;
	
#if !defined DEBUG_AS_REALTIME
	ofxUDPManager udp_receive_; // 通常使用のみ
#endif
	
	ofxUDPManager udp_send_;
	ofxUDPManager udp_send_soundsync_point_size_;
	
	bool b_change_contents = false;
	
	/********************
	********************/
	enum{
		kMaxNumDirs_ = 10,
	};
	
	const int kNumDirs_;
	FilesInDir files_in_dir_[kMaxNumDirs_];
	
	int dir_id_ = 0;
	
	PointSize sound_sync_point_size[kMaxNumDirs_] = {
		PointSize(0.07, 0.03),	// 0
		PointSize(0.043, 0.02),	// 1
		PointSize(0.07, 0.03),	// 2
		PointSize(0.07, 0.03),	// 3
		
		PointSize(0.07, 0.03),	// 4
		PointSize(0.07, 0.03),	// 5
		PointSize(0.07, 0.03),	// 6
		PointSize(0.07, 0.03),	// 7
		PointSize(0.07, 0.03),	// 8
		PointSize(0.07, 0.03),	// 9
	};
	
	/********************
	********************/
	const int kSendAtOnce_;
	const float kFps_;
	
	int num_points_in_this_frame_;
	int num_points_to_send_;
	int num_packets_;
	
	vector<ofVec3f> positions_;
	vector<ofVec3f> pos_thinned_;
	
	/********************
	********************/
	// int t_from_ = 0;
	
	int t_elapsed_sec_;
	int t_elapsed_h_;
	int t_elapsed_m_;
	
	/********************
	********************/
	enum class WindowSize{
		kWidth_		= 300,
		kHeight_	= 500,
	};
	
	/********************
	********************/
	enum class State{
		kWait_,
		kRun_,
	};
	
	State state_ = State::kWait_;
	
	/********************
	********************/
	ofTrueTypeFont font_M_;
	
	
	/****************************************
	****************************************/
	void SetNextContents();
	void SetupUdp();
	void UpdatePosition();
	void PrepAndSendUdp();
	void PrepAndSendUdp_SoundSyncPointSize();
	void CalElapsedTime();
	void Align_StringOfData(string& s);
	bool CheckIfContentsExist(const string& str_line);
	void TryToReceiveUdpMessage();
	
	static bool CmpVector3ByX(const ofVec3f &a, const ofVec3f &b);
	static bool CmpVector3ByZ(const ofVec3f &a, const ofVec3f &b);
	static bool CmpVector3ByDistance(const ofVec3f &a, const ofVec3f &b);
	
	
public:
	ofApp(int max_points_in_1_frame, int send_at_once, int num_log_dirs, float fps);
	~ofApp();
	
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y ) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;
};
