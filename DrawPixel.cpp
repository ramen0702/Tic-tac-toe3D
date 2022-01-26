#include "DxLib.h"

void draw(int data[][3][3],int angle);//描画関数
int put_mark(int angle,int ClickX, int ClickY, int put_cnt, int data[][3][3]);//マーク配置関数
int judge(int data[][3][3], int check);//勝利判定関数
void reset(int data[][3][3]);//初期化関数
// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //-------------------------------変数宣言-----------------------------------------------------------
    //マウス関連の変数
    int ClickX, ClickY, Button, LogType;//クリックしたx座標、y座標、ボタンを押したか離したか、ログを保存するか

    //マス情報3次元配列。0は設置可能。3は不可。1はプレイヤーA、2はプレイヤーB
    int square[3][3][3] = { {{3,3,3},{3,3,3},{0,0,0}},{{3,3,3},{3,3,3},{0,0,0}},{{3,3,3},{3,3,3},{0,0,0}}};

    //その他
    int Color = GetColor(255, 255, 255);//カラー変数。白。
    int put_cnt = 1;//置かれたマークの個数のカウントを行う変数。特殊数値として-1はA、-2はBの勝利を表す
    int angle = 1;//どこを向いているか。1～4は側面、5は上面、6は底面
    int flag = 1;  //画面遷移変数。1ならタイトル画面、2なら説明画面、3ならプレイ画面
    int stop = 0;  //画面遷移が行き過ぎないように一度マウスを離すまでストップをかける変数

    //-------------------------------起動処理-----------------------------------------------------------
    if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
    {
        return -1;			// エラーが起きたら直ちに終了
    }

    //-------------------------------初期設定-----------------------------------------------------------
    SetMouseDispFlag(true);   //マウスカーソルを表示
    ChangeWindowMode(true);  //ウィンドウ表示
    SetFontSize(64);  //フォントサイズの指定
    SetWindowUserCloseEnableFlag(FALSE);  //×ボタンを禁止
    
    //警告文
    DrawFormatString(225, 125, GetColor(255, 0, 0), "!警告!");
    DrawFormatString(175, 200, GetColor(255, 0, 0), "本ソフトは");
    DrawFormatString(175, 275, GetColor(255, 0, 0), "音が出ます");
    WaitTimer(6000);  //6秒待つ
    PlaySoundFile("music.mp3", DX_PLAYTYPE_LOOP);  //音楽をループ再生

    //-------------------------------ループ開始-----------------------------------------------------------
    while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)   //escで終了
    {
        switch (flag) {
        case 1:

            //-------------------------------タイトル画面-----------------------------------------------------------
            ClearDrawScreen();  // 画面をクリア

            //テキストを表示
            DrawFormatString(150, 125, Color, "三目並べ3D");
            DrawFormatString(50, 325, GetColor(0, 0, 255), "-Click to start-");

            //クリックされたら画面遷移
            if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, true) == 0)  // マウスのボタンが押されたり離されたりしたかどうか
            {
                if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)  //左クリックをしたら
                {
                    stop = 1;
                }
            }else{
                if (stop) {
                    flag = 2;
                    stop = 0;
                    SetFontSize(32);  //フォントサイズの指定
                }
            }
            ScreenFlip();  //裏画面を表画面へ表示
            break;
        case 2:

            //-------------------------------説明画面-----------------------------------------------------------
            ClearDrawScreen();  // 画面をクリア

            //テキストを表示
            DrawFormatString(0, 25, Color, "～ルール説明～");
            DrawFormatString(0, 75, Color, "・3D空間で三目並べをします");
            DrawFormatString(0, 125, Color, "・cpu戦ではなくPvP戦です");
            DrawFormatString(0, 175, Color, "・交互にマスをクリックしてください");
            DrawFormatString(0, 225, Color, "・一つマークを置くとその上のマスに");
            DrawFormatString(0, 275, Color, "　マークを置けるようになります");
            DrawFormatString(0, 325, Color, "・三つ揃えると勝ちです");
            DrawFormatString(0, 375, Color, "・三角を押すと視点移動ができます");
            DrawFormatString(0, 425, GetColor(255, 0, 0), "・Escキーで終了。×ボタンは不可。");

            //クリックされたら画面遷移
            if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, true) != 0)  // マウスのボタンが押されたり離されたりしたかどうか
            {
                if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0)  //左クリックをしたら
                {
                    stop = 1;
                }
            }
            else {
                if (stop) {
                    flag = 3;
                    stop = 0;
                }
            }
            ScreenFlip();  //裏画面を表画面へ表示
            break;
        case 3:

            //-------------------------------メイン処理-----------------------------------------------------------
            ClearDrawScreen();  // 画面をクリア

            draw(square, angle);//描画関数

            //-------------------------------テキスト処理など-----------------------------------------------------------
            if (put_cnt == -1) {
                //Aさん勝利処理
                DrawFormatString(0, 25, GetColor(255, 0, 0), "Aさんの勝利!!");
                DrawFormatString(0, 60, GetColor(0, 0, 255), "-Enter to restart-");

                //エンターを押したら画面遷移
                if (CheckHitKey(KEY_INPUT_RETURN) == 1)  //エンターを押したら
                {
                    flag = 1;
                    put_cnt = 1;
                    angle = 1;
                    reset(square);  //初期化関数で配列を初期化
                    SetFontSize(64);  //フォントサイズの指定
                }
            }
            else if (put_cnt == -2) {
                //Bさん勝利処理
                DrawFormatString(0, 25, GetColor(0, 255, 0), "Bさんの勝利!!");
                DrawFormatString(0, 60, GetColor(0, 0, 255), "-Enter to restart-");

                //エンターを押したら画面遷移
                if (CheckHitKey(KEY_INPUT_RETURN) == 1)  //エンターを押したら
                {
                    flag = 1;
                    put_cnt = 1;
                    angle = 1;
                    SetFontSize(64);  //フォントサイズの指定
                    reset(square);  //初期化関数で配列を初期化
                }
            }
            else if (put_cnt == 27) {
                //引き分け処理
                DrawFormatString(0, 25, Color, "引き分け");
                DrawFormatString(0, 60, GetColor(0, 0, 255), "-Enter to restart-");

                //エンターを押したら画面遷移
                if (CheckHitKey(KEY_INPUT_RETURN) == 1)  //エンターを押したら
                {
                    flag = 1;
                    put_cnt = 1;
                    angle = 1;
                    SetFontSize(64);  //フォントサイズの指定
                    reset(square);  //初期化関数で配列を初期化
                }
            }
            //AさんのターンなのかBさんのターンなのか表示
            else if (put_cnt % 2 != 0) {
                DrawFormatString(0, 25, GetColor(255, 0, 0), "Aさんのターン");
            }
            else {
                DrawFormatString(0, 25, GetColor(0, 255, 0), "Bさんのターン");
            }

            ScreenFlip();  //裏画面を表画面へ表示

            //-------------------------------マウス処理-----------------------------------------------------------
            if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, true) == 0)  // マウスのボタンが押されたり離されたりしたかどうか
            {
                if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)  //左クリックをしたら
                {
                    if (put_cnt != -1 && put_cnt != -2) {//どちらかが既に勝利していた場合、クリックを無効
                        put_cnt = put_mark(angle, ClickX, ClickY, put_cnt, square);//マークを置き、カウントを返す
                    }
                    //-------------------------------カメラ処理-----------------------------------------------------------
                    //現在どこを見ているか
                    switch (angle) {
                    case 1:
                        //どこを押したか
                        if ((300 <= ClickX && ClickX <= 350) && (0 <= ClickY && ClickY <= 25)) {
                            angle = 5;  //視点を変更
                        }
                        else if ((0 <= ClickX && ClickX <= 25) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 2;
                        }
                        else if ((300 <= ClickX && ClickX <= 350) && (450 <= ClickY && ClickY <= 475)) {
                            angle = 6;
                        }
                        else if ((600 <= ClickX && ClickX <= 625) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 4;
                        }
                        break;
                    case 2:
                        if ((0 <= ClickX && ClickX <= 25) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 3;
                        }
                        else if ((600 <= ClickX && ClickX <= 625) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 1;
                        }
                        break;
                    case 3:
                        if ((0 <= ClickX && ClickX <= 25) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 4;
                        }
                        else if ((600 <= ClickX && ClickX <= 625) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 2;
                        }
                        break;
                    case 4:
                        if ((0 <= ClickX && ClickX <= 25) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 1;
                        }
                        else if ((600 <= ClickX && ClickX <= 625) && (225 <= ClickY && ClickY <= 275)) {
                            angle = 3;
                        }
                        break;
                    case 5:
                        if ((300 <= ClickX && ClickX <= 350) && (450 <= ClickY && ClickY <= 475)) {
                            angle = 1;
                        }
                        break;
                    case 6:
                        if ((300 <= ClickX && ClickX <= 350) && (0 <= ClickY && ClickY <= 25)) {
                            angle = 1;
                        }
                        break;
                    default:
                        break;
                    }
                    
                }
            }
            break;
        default:
            break;
        }
    }
    //-------------------------------終了処理-----------------------------------------------------------
    DxLib_End();  // ＤＸライブラリ使用の終了処理
    return 0;  // ソフトの終了 
}

//-------------------------------描画関数-----------------------------------------------------------
void draw(int data[][3][3],int angle) {
    int Color0 = GetColor(0, 0, 255);//置ける色
    int Color1 = GetColor(255, 0, 0);//Aが置いた色
    int Color2 = GetColor(0, 255, 0);//Bが置いた色
    int Color3 = GetColor(255, 255, 0);//置けない色
    int Color = GetColor(255, 255, 255);
    int i, j, x = 180, y = 100;
    for (i = 0; i < 3; i++) {
        y = 100;
        for (j = 0; j < 3; j++) {
            switch (angle) {
            case 1:
                DrawTriangle(325, 0, 300, 25,350, 25, Color, TRUE);
                DrawTriangle(0, 250, 25, 225, 25, 275, Color, TRUE);
                DrawTriangle(325, 475, 300, 450, 350, 450, Color, TRUE);
                DrawTriangle(625, 250, 600, 225, 600, 275, Color, TRUE);
                if (data[i][j][0] == 0) {
                    DrawBox(x, y, x + 90, y + 90, Color0, TRUE);
                }
                else if (data[i][j][0] == 1) {
                    DrawBox(x, y, x + 90, y + 90, Color1, TRUE);
                }
                else if (data[i][j][0] == 2) {
                    DrawBox(x, y, x + 90, y + 90, Color2, TRUE);
                }
                else {
                    DrawBox(x, y, x + 90, y + 90, Color3, TRUE);
                }
                break;
            case 2:
                DrawTriangle(0, 250, 25, 225, 25, 275, Color, TRUE);
                DrawTriangle(625, 250, 600, 225, 600, 275, Color, TRUE);
                if (data[0][j][2-i] == 0) {
                    DrawBox(x, y, x + 90, y + 90, Color0, TRUE);
                }
                else if (data[0][j][2-i] == 1) {
                    DrawBox(x, y, x + 90, y + 90, Color1, TRUE);
                }
                else if (data[0][j][2-i] == 2) {
                    DrawBox(x, y, x + 90, y + 90, Color2, TRUE);
                }
                else {
                    DrawBox(x, y, x + 90, y + 90, Color3, TRUE);
                }
                break;
            case 3:
                DrawTriangle(0, 250, 25, 225, 25, 275, Color, TRUE);
                DrawTriangle(625, 250, 600, 225, 600, 275, Color, TRUE);
                if (data[2-i][j][2] == 0) {
                    DrawBox(x, y, x + 90, y + 90, Color0, TRUE);
                }
                else if (data[2-i][j][2] == 1) {
                    DrawBox(x, y, x + 90, y + 90, Color1, TRUE);
                }
                else if (data[2-i][j][2] == 2) {
                    DrawBox(x, y, x + 90, y + 90, Color2, TRUE);
                }
                else {
                    DrawBox(x, y, x + 90, y + 90, Color3, TRUE);
                }
                break;
            case 4:
                DrawTriangle(0, 250, 25, 225, 25, 275, Color, TRUE);
                DrawTriangle(625, 250, 600, 225, 600, 275, Color, TRUE);
                if (data[2][j][i] == 0) {
                    DrawBox(x, y, x + 90, y + 90, Color0, TRUE);
                }
                else if (data[2][j][i] == 1) {
                    DrawBox(x, y, x + 90, y + 90, Color1, TRUE);
                }
                else if (data[2][j][i] == 2) {
                    DrawBox(x, y, x + 90, y + 90, Color2, TRUE);
                }
                else {
                    DrawBox(x, y, x + 90, y + 90, Color3, TRUE);
                }
                break;
            case 5:
                DrawTriangle(325, 475, 300, 450, 350, 450, Color, TRUE);
                if (data[i][0][2-j] == 0) {
                    DrawBox(x, y, x + 90, y + 90, Color0, TRUE);
                }
                else if (data[i][0][2-j] == 1) {
                    DrawBox(x, y, x + 90, y + 90, Color1, TRUE);
                }
                else if (data[i][0][2-j] == 2) {
                    DrawBox(x, y, x + 90, y + 90, Color2, TRUE);
                }
                else {
                    DrawBox(x, y, x + 90, y + 90, Color3, TRUE);
                }
                break;
            case 6:
                DrawTriangle(325, 0, 300, 25, 350, 25, Color, TRUE);
                if (data[i][2][j] == 0) {
                    DrawBox(x, y, x + 90, y + 90, Color0, TRUE);
                }
                else if (data[i][2][j] == 1) {
                    DrawBox(x, y, x + 90, y + 90, Color1, TRUE);
                }
                else if (data[i][2][j] == 2) {
                    DrawBox(x, y, x + 90, y + 90, Color2, TRUE);
                }
                else {
                    DrawBox(x, y, x + 90, y + 90, Color3, TRUE);
                }
                break;
            default:
                break;
            }
            y += 100;
        }
        x += 100;
    }
}

//-------------------------------マーク配置関数-----------------------------------------------------------
int put_mark(int angle,int ClickX, int ClickY, int put_cnt, int data[][3][3]) {
    int i, j, x = 180, y = 100, flag = 0;
    for (i = 0; i < 3; i++) {
        y = 100;
        for (j = 0; j < 3; j++) {
            if (x <= ClickX && ClickX <= x + 90 && y <= ClickY && ClickY <= y + 90) {
                switch (angle) {
                case 1: 
                    if (data[i][j][0] == 0) {
                        if (put_cnt % 2 != 0) {
                            data[i][j][0] = 1;
                            flag = judge(data, 1);
                        }
                        else {
                            data[i][j][0] = 2;
                            flag = judge(data, 2);
                        }
                        if (j != 0) {
                            data[i][j - 1][0] = 0;
                        }
                        if (flag) {
                            return (flag * -1);
                        }
                        else {
                            return put_cnt + 1;
                        }
                    }
                    break;
                case 2:
                    if (data[0][j][2-i] == 0) {
                        if (put_cnt % 2 != 0) {
                            data[0][j][2-i] = 1;
                            flag = judge(data, 1);
                        }
                        else {
                            data[0][j][2-i] = 2;
                            flag = judge(data, 2);
                        }
                        if (j != 0) {
                            data[0][j-1][2-i] = 0;
                        }
                        if (flag) {
                            return (flag * -1);
                        }
                        else {
                            return put_cnt + 1;
                        }
                    }
                    break;
                case 3:
                    if (data[2-i][j][2] == 0) {
                        if (put_cnt % 2 != 0) {
                            data[2-i][j][2] = 1;
                            flag = judge(data, 1);
                        }
                        else {
                            data[2-i][j][2] = 2;
                            flag = judge(data, 2);
                        }
                        if (j != 0) {
                            data[2-i][j - 1][2] = 0;
                        }
                        if (flag) {
                            return (flag * -1);
                        }
                        else {
                            return put_cnt + 1;
                        }
                    }
                    break;
                case 4:
                    if (data[2][j][i] == 0) {
                        if (put_cnt % 2 != 0) {
                            data[2][j][i] = 1;
                            flag = judge(data, 1);
                        }
                        else {
                            data[2][j][i] = 2;
                            flag = judge(data, 2);
                        }
                        if (j != 0) {
                            data[2][j - 1][i] = 0;
                        }
                        if (flag) {
                            return (flag * -1);
                        }
                        else {
                            return put_cnt + 1;
                        }
                    }
                    break;
                case 5:
                    if (data[i][0][2 - j] == 0) {
                        if (put_cnt % 2 != 0) {
                            data[i][0][2 - j] = 1;
                            flag = judge(data, 1);
                        }
                        else {
                            data[i][0][2 - j] = 2;
                            flag = judge(data, 2);
                        }
                        if (flag) {
                            return (flag * -1);
                        }
                        else {
                            return put_cnt + 1;
                        }
                    }
                    break;
                case 6:
                    if (data[i][2][j] == 0) {
                        if (put_cnt % 2 != 0) {
                            data[i][2][j] = 1;
                            flag = judge(data, 1);
                        }
                        else {
                            data[i][2][j] = 2;
                            flag = judge(data, 2);
                        }
                        if (i == 1 && j == 1) {
                            data[i][0][j] = 0;
                        }
                        else {
                            data[i][1][j] = 0;
                        }
                        if (flag) {
                            return (flag * -1);
                        }
                        else {
                            return put_cnt + 1;
                        }
                    }
                    break;
                }
            }
            y += 100;
        }
        x += 100;
    }
    return put_cnt;
}

//-------------------------------勝敗判定関数-----------------------------------------------------------
int judge(int data[][3][3], int check) {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            if (data[j][i][0] == check && data[j][i][1] == check && data[j][i][2] == check) {
                return  check;
            }
            if (data[j][0][i] == check && data[j][1][i] == check && data[j][2][i] == check) {
                return  check;
            }
            if (data[0][i][j] == check && data[1][i][j] == check && data[2][i][j] == check) {
                return  check;
            }
            if (data[i][0][j] == check && data[i][1][j] == check && data[i][2][j] == check) {
                return  check;
            }
            if (data[i][j][0] == check && data[i][j][1] == check && data[i][j][2] == check) {
                return  check;
            }
            if (data[1][j][i] == check && data[2][j][i] == check && data[3][j][i] == check) {
                return  check;
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        if (data[i][0][0] == check && data[i][1][1] == check && data[i][2][2] == check) {
            return  check;
        }
        if (data[i][0][2] == check && data[i][1][1] == check && data[i][2][0] == check) {
            return  check;
        }
        if (data[0][0][i] == check && data[1][1][i] == check && data[2][2][i] == check) {
            return  check;
        }
        if (data[2][0][i] == check && data[1][1][i] == check && data[0][2][i] == check) {
            return  check;
        }
        if (data[0][i][0] == check && data[1][i][1] == check && data[2][i][2] == check) {
            return  check;
        }
        if (data[2][i][0] == check && data[1][i][1] == check && data[0][i][2] == check) {
            return  check;
        }
    }
    return  0;
}

//-------------------------------配列初期化関数-----------------------------------------------------------
void reset(int data[][3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (j != 2) {
                    data[i][j][k] = 3;
                }
                else {
                    data[i][j][k] = 0;
                }
            }
        }
    }
}