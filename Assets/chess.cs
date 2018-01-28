using UnityEngine;
using System.Collections;

public class chess : MonoBehaviour {

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
        int flag = 0;
        //如果当前该白棋落子，标定黑棋刚刚下完一步，此时应该判断黑棋是否获胜  
        if (chessturn == turn.white)
        {
            for (int i = 0; i < 11; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    if (j < 4)
                    {
                        //横向  
                        if (chessstate[i, j] == 1 && chessstate[i, j + 1] == 1 && chessstate[i, j + 2] == 1 && chessstate[i, j + 3] == 1 && chessstate[i, j + 4] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                        //纵向  
                        if (chessstate[i, j] == 1 && chessstate[i + 1, j] == 1 && chessstate[i + 2, j] == 1 && chessstate[i + 3, j] == 1 && chessstate[i + 4, j] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                        //右斜线  
                        if (chessstate[i, j] == 1 && chessstate[i + 1, j + 1] == 1 && chessstate[i + 2, j + 2] == 1 && chessstate[i + 3, j + 3] == 1 && chessstate[i + 4, j + 4] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                        //左斜线  
                        //if (chessState[i, j] == 1 && chessState[i + 1, j - 1] == 1 && chessState[i + 2, j - 2] == 1 && chessState[i + 3, j - 3] == 1 && chessState[i + 4, j - 4] == 1)  
                        //{  
                        //    flag = 1;  
                        //    return flag;  
                        //}  
                    }
                    else if (j >= 4 && j < 11)
                    {
                        //横向  
                        if (chessstate[i, j] == 1 && chessstate[i, j + 1] == 1 && chessstate[i, j + 2] == 1 && chessstate[i, j + 3] == 1 && chessstate[i, j + 4] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                        //纵向  
                        if (chessstate[i, j] == 1 && chessstate[i + 1, j] == 1 && chessstate[i + 2, j] == 1 && chessstate[i + 3, j] == 1 && chessstate[i + 4, j] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                        //右斜线  
                        if (chessstate[i, j] == 1 && chessstate[i + 1, j + 1] == 1 && chessstate[i + 2, j + 2] == 1 && chessstate[i + 3, j + 3] == 1 && chessstate[i + 4, j + 4] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                        //左斜线  
                        if (chessstate[i, j] == 1 && chessstate[i + 1, j - 1] == 1 && chessstate[i + 2, j - 2] == 1 && chessstate[i + 3, j - 3] == 1 && chessstate[i + 4, j - 4] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                    }
                    else
                    {
                        //横向  
                        //if (chessState[i, j] == 1 && chessState[i, j + 1] == 1 && chessState[i, j + 2] == 1 && chessState[i, j + 3] == 1 && chessState[i, j + 4] == 1)  
                        //{  
                        //    flag = 1;  
                        //    return flag;  
                        //}  
                        //纵向  
                        if (chessstate[i, j] == 1 && chessstate[i + 1, j] == 1 && chessstate[i + 2, j] == 1 && chessstate[i + 3, j] == 1 && chessstate[i + 4, j] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                        //右斜线  
                        //if (chessState[i, j] == 1 && chessState[i + 1, j + 1] == 1 && chessState[i + 2, j + 2] == 1 && chessState[i + 3, j + 3] == 1 && chessState[i + 4, j + 4] == 1)  
                        //{  
                        //    flag = 1;  
                        //    return flag;  
                        //}  
                        //左斜线  
                        if (chessstate[i, j] == 1 && chessstate[i + 1, j - 1] == 1 && chessstate[i + 2, j - 2] == 1 && chessstate[i + 3, j - 3] == 1 && chessstate[i + 4, j - 4] == 1)
                        {
                            flag = 1;
                            return flag;
                        }
                    }

                }
            }
            for (int i = 11; i < 15; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    //只需要判断横向    
                    if (chessstate[i, j] == 1 && chessstate[i, j + 1] == 1 && chessstate[i, j + 2] == 1 && chessstate[i, j + 3] == 1 && chessstate[i, j + 4] == 1)
                    {
                        flag = 1;
                        return flag;
                    }
                }
            }
        }
        //如果当前该黑棋落子，标定白棋刚刚下完一步，此时应该判断白棋是否获胜  
        else if (chessturn == turn.black)
        {
            for (int i = 0; i < 11; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    if (j < 4)
                    {
                        //横向  
                        if (chessstate[i, j] == -1 && chessstate[i, j + 1] == -1 && chessstate[i, j + 2] == -1 && chessstate[i, j + 3] == -1 && chessstate[i, j + 4] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                        //纵向  
                        if (chessstate[i, j] == -1 && chessstate[i + 1, j] == -1 && chessstate[i + 2, j] == -1 && chessstate[i + 3, j] == -1 && chessstate[i + 4, j] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                        //右斜线  
                        if (chessstate[i, j] == -1 && chessstate[i + 1, j + 1] == -1 && chessstate[i + 2, j + 2] == -1 && chessstate[i + 3, j + 3] == -1 && chessstate[i + 4, j + 4] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                        //左斜线  
                        //if (chessState[i, j] == -1 && chessState[i + 1, j - 1] == -1 && chessState[i + 2, j - 2] == -1 && chessState[i + 3, j - 3] == -1 && chessState[i + 4, j - 4] == -1)  
                        //{  
                        //    flag = -1;  
                        //    return flag;  
                        //}  
                    }
                    else if (j >= 4 && j < 11)
                    {
                        //横向  
                        if (chessstate[i, j] == -1 && chessstate[i, j + 1] == -1 && chessstate[i, j + 2] == -1 && chessstate[i, j + 3] == -1 && chessstate[i, j + 4] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                        //纵向  
                        if (chessstate[i, j] == -1 && chessstate[i + 1, j] == -1 && chessstate[i + 2, j] == -1 && chessstate[i + 3, j] == -1 && chessstate[i + 4, j] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                        //右斜线  
                        if (chessstate[i, j] == -1 && chessstate[i + 1, j + 1] == -1 && chessstate[i + 2, j + 2] == -1 && chessstate[i + 3, j + 3] == -1 && chessstate[i + 4, j + 4] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                        //左斜线  
                        if (chessstate[i, j] == -1 && chessstate[i + 1, j - 1] == -1 && chessstate[i + 2, j - 2] == -1 && chessstate[i + 3, j - 3] == -1 && chessstate[i + 4, j - 4] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                    }
                    else
                    {
                        //横向  
                        //if (chessState[i, j] == -1 && chessState[i, j + 1] ==- 1 && chessState[i, j + 2] == -1 && chessState[i, j + 3] == -1 && chessState[i, j + 4] == -1)  
                        //{  
                        //    flag = -1;  
                        //    return flag;  
                        //}  
                        //纵向  
                        if (chessstate[i, j] == -1 && chessstate[i + 1, j] == -1 && chessstate[i + 2, j] == -1 && chessstate[i + 3, j] == -1 && chessstate[i + 4, j] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                        //右斜线  
                        //if (chessState[i, j] == -1 && chessState[i + 1, j + 1] == -1 && chessState[i + 2, j + 2] == -1 && chessState[i + 3, j + 3] == -1 && chessState[i + 4, j + 4] == -1)  
                        //{  
                        //    flag = -1;  
                        //    return flag;  
                        //}  
                        //左斜线  
                        if (chessstate[i, j] == -1 && chessstate[i + 1, j - 1] == -1 && chessstate[i + 2, j - 2] == -1 && chessstate[i + 3, j - 3] == -1 && chessstate[i + 4, j - 4] == -1)
                        {
                            flag = -1;
                            return flag;
                        }
                    }
                }
            }
            for (int i = 11; i < 15; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    //只需要判断横向    
                    if (chessstate[i, j] == -1 && chessstate[i, j + 1] == -1 && chessstate[i, j + 2] == -1 && chessstate[i, j + 3] == -1 && chessstate[i, j + 4] == -1)
                    {
                        flag = -1;
                        return flag;
                    }
                }
            }
        }
        return flag;
    }      
}
