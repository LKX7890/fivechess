using UnityEngine;
using System.Collections;

public class chess : MonoBehaviour {
    enum DIR_TYPE
    {
        DIR_TYPE_0,
        DIR_TYPE_1,
        DIR_TYPE_2,
        DIR_TYPE_3,
    }

    static const int MAX_DIR_TYPE_COUNT = 4;
    static const int MIN_WIN_DIR_COUNT = 9;

    // 锚点和场景对象
    public GameObject LeftTop;
    public GameObject RightTop;
    public GameObject LeftBottom;
    public GameObject RightBottom;
    public Camera cam;
    public Texture2D white;
    public Texture2D black;
    public Texture2D blackwin;
    public Texture2D whitewin;

    // 锚点映射的屏幕位置
    Vector3 LTPos;
    Vector3 RTPos;
    Vector3 LBPos;
    Vector3 RBPos;

    // 格子长宽
    float gridwidth = 1;
    float gridheight = 1;
    float mingriddis;

    // 棋盘存储结构
    Vector2[,] chesspos;
    int[,] chessstate;

    // 鼠标点击位置
    Vector3 pointpos;
    int cur_x = 0;
    int cur_y = 0;

    // 转换类型
    enum turn { black, white };
    turn chessturn;

    int winner = 0;
    bool isplaying = true;

	void Start () {
        chesspos = new Vector2[15, 15];
        chessstate = new int[15, 15];
        chessturn = turn.black;
	}
	
	// Update is called once per frame
    void Update()
    {
        // 计算锚点位置
        LTPos = cam.WorldToScreenPoint(LeftTop.transform.position);
        RTPos = cam.WorldToScreenPoint(RightTop.transform.position);
        LBPos = cam.WorldToScreenPoint(LeftBottom.transform.position);
        RBPos = cam.WorldToScreenPoint(RightBottom.transform.position);

        // 计算网格宽度
        gridwidth = (RTPos.x - LTPos.x) / 14;
        gridheight = (LTPos.y - LBPos.y) / 14;
        mingriddis = gridwidth < gridheight ? gridwidth : gridheight;

        // 初始化棋盘坐标
        for (int i = 0; i < 15; ++i)
        {
            for (int j = 0; j < 15; ++j)
            {
                chesspos[i, j] = new Vector2(LBPos.x + gridwidth * i, LBPos.y + gridheight * j);
            }
        }

        // 获取鼠标点击的点，下棋
        if (isplaying && Input.GetMouseButtonDown(0))
        {
            pointpos = Input.mousePosition;
            for (int i = 0; i < 15; ++i)
            {
                for (int j = 0; j < 15; ++j)
                {
                    if (Dis(pointpos, chesspos[i, j]) < mingriddis / 2 && chessstate[i, j] == 0)
                    {
                        chessstate[i, j] = chessturn == turn.black ? 1 : -1;
                        chessturn = chessturn == turn.black ? turn.white : turn.black;
                        cur_x = i;
                        cur_y = j;
                    }
                }
            }

            // 判断结果
            int result = Result();
            if (result == 1)
            {
                Debug.Log("黑棋胜");
                winner = 1;
                isplaying = false;
            }
            else if (result == -1)
            {
                Debug.Log("白旗胜");
                winner = -1;
                isplaying = false;
            }
        }

        // 输入空格开始
        if (Input.GetKeyDown(KeyCode.Space))
        {
            for (int i = 0; i < 15; ++i)
            {
                for (int j = 0; j < 15; ++j)
                {
                    chessstate[i, j] = 0;
                }
            }

            isplaying = true;
            chessturn = turn.black;
            winner = 0;
        }
    }

    // 计算距离
    float Dis(Vector3 mpos, Vector2 gridpos)
    {
        return Mathf.Sqrt(Mathf.Pow(mpos.x - gridpos.x, 2) + Mathf.Pow(mpos.y - gridpos.y, 2)); 
    }

    // 绘制
    void OnGUI()
    {
        for (int i = 0; i < 15; ++i)
        {
            for (int j = 0; j < 15; ++j)
            {
                if (chessstate[i, j] == 1)
                {
                    GUI.DrawTexture(new Rect(chesspos[i, j].x - gridwidth / 2,
                    Screen.height - chesspos[i, j].y - gridheight / 2, gridwidth, gridheight), black);
                }
                
                if (chessstate[i, j] == -1)
                {
                    GUI.DrawTexture(new Rect(chesspos[i, j].x - gridwidth / 2, 
                        Screen.height - chesspos[i, j].y - gridheight / 2, gridwidth, gridheight), white);
                }    
            }
        }

        if (winner == 1)
        {
            GUI.DrawTexture(new Rect(Screen.width * 0.25f, Screen.height * 0.25f, Screen.width * 0.5f, Screen.height * 0.25f), blackwin);
        }

        if (winner == -1)
        {
            GUI.DrawTexture(new Rect(Screen.width * 0.25f, Screen.height * 0.25f, Screen.width * 0.5f, Screen.height * 0.25f), whitewin);
        }
    }

    //检测是够获胜的函数，不含黑棋禁手检测  
    int Result()
    {
        if (!CheckCanWin()) return 0;
        if (turn.black == chessturn)
        {
            return -11;
        }
        else
        {
            return 1;
        }
    }

    int CalChessmanCount(int cur_x, int cur_y, int begin_x, int begin_y, int x_add, int y_add)
    {
        int count = 0;
        int x = cur_x - begin_x;
        int y = cur_y - begin_y;
        for (int i = 0; i < MIN_WIN_DIR_COUNT; ++i)
        {
            if (x < 0 || y < 0)
            {
                x += x_add;
                y += y_add;
                continue;
            }

            if (chessstate[x,y] == 1 || chessstate[x,y] == -1)
            {
                ++count;
            }

            if (5 >= count)
            {
                return count;
            }
        }

        return count;
    }
    bool CheckCanWin()
    {
        for (int i = 0; i < MAX_DIR_TYPE_COUNT; ++i)
        {
            int begin_x = 0;
            int begin_y = 0;
            int x_add = 0;
            int y_add = 0;
            int count = 0;
            switch (i)
            {
                case 0:
                    {
                        begin_x = -4;
                        begin_y = 0;
                        x_add = 1;
                        y_add = 0;
                        count = CalChessmanCount(cur_x, cur_y, begin_x, begin_y, x_add, y_add);
                    }
                    break;

                case 1:
                    {
                        begin_x = 0;
                        begin_y = -4;
                        x_add = 0;
                        y_add = 1;
                        count = CalChessmanCount(cur_x, cur_y, begin_x, begin_y, x_add, y_add);
                    }
                    break;

                case 2:
                    {
                        begin_x = -4;
                        begin_y = -4;
                        x_add = 1;
                        y_add = 1;
                        count = CalChessmanCount(cur_x, cur_y, begin_x, begin_y, x_add, y_add);
                    }
                    break;

                case 3:
                    {
                        begin_x = -4;
                        begin_y = 4;
                        x_add = 1;
                        y_add = -1;
                        count = CalChessmanCount(cur_x, cur_y, begin_x, begin_y, x_add, y_add);
                    }
                    break;
            }

            if (5 >= count) return true;
        }

        return false;
    }
}


