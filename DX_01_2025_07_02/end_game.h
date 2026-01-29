// ==============================================================
// end_game.h	
// ゲーム終了制御
// 
// 制作者:村上和樹		制作日付：2025/12/01	
// 更新者:村上和樹		更新日付：2025/12/01
// ==============================================================
#ifndef END_GAME_H
#define END_GAME_H

class EndGame {
public:
	enum End {
		None,
		GameClear,
		GameOver,
	};

private:
	End m_GameState = None;

public:
	void Initialize(void);
	void Finalize(void);
	void Update(double elpased_time);
	void Draw(void);


	const End& GetEnd() const { return m_GameState; }
	void SetEnd(End GameState) { m_GameState = GameState; }
};
EndGame* GetEndGame();

#endif
