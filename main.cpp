#include <Novice.h>
#include "MT40101.h"
#include "MT40102.h"
#include "MT4_0103.h"
#include "MT4_0104.h"
#include "MT40105.h"
const char kWindowTitle[] = "LE2A_04_カシワギハルカ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	/*MT40101 mt40101;*/
	/*MT40102 mt40102;*/
	/*MT4_0103 mt0103;*/
	/*MT4_0104 mt0104;*/	
	MT40105 mt0105;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		/*mt40101.Update();*/
		/*mt40102.Update();*/
		/*mt0103.Update();*/
		/*mt0104.Update();*/
		mt0105.Update();

		///
		/// ↑更新処理ここまで
		///
		
		/*mt40101.Draw();*/
		/*mt40102.Draw();*/
		/*mt0103.Draw();*/
		/*mt0104.Draw();*/
		mt0105.Draw();

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
