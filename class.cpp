// ---------------------------------------------------------------------------

#pragma hdrstop

#include "class.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

Game::Game(TImage *img, TImageList *list, TImage *desktop) {
	this->device = img->Canvas;
    device->Brush->Style = bsClear;
	device->Pen->Width = 2;

	Card_W = list->Width;
	Card_H = list->Height;

	Desk_Size = desktop->Height;
	this->desktop = new TPicture();
	this->desktop = desktop->Picture;

	BOX_0_1 = new TPoint(45, 5);
	BOX_2_8 = new TPoint(BOX_0_1->x, BOX_0_1->y + Card_H + Card_sm);
	BOX_9_12 = new TPoint(BOX_0_1->x + Card_W * 3 + Card_sm * 3, BOX_0_1->y);

	// ���������� ��� ��������� ������ � �������� ���������
	for (int s = 0; s < 13; s++)
		for (int n = 0; n < 53; n++)
			sCard_Box[s][n].Value = -1;

	// �������� � �������� ���� �����
	CARD_SUIT suit[4] = {SPADES, HEARTS, CLUBS, DIAMONDS};
	for (int i = 0; i < 52; i++) {
		sCard_Box[0][i].Value = i / 4; // ������ �����
		sCard_Box[0][i].Suit = suit[i % 4];
		sCard_Box[0][i].Visible = true;
		sCard_Box[0][i].Pict = new TPicture();
		list->GetBitmap(i, sCard_Box[0][i].Pict->Bitmap);
	}
	back = new TPicture();
	list->GetBitmap(52, back->Bitmap);

	// ������������ ����� � 0 �����
	for (int i = 0; i < 52; i++)
		std::swap(sCard_Box[0][i], sCard_Box[0][rand() % 52]);

	// ������ ������������ ����� �� ������
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j <= i; j++) {
			// ���������� �����
			if (MoveCard(0, i + 2) == false)
				// ���� ����� � ����� 0 - ������ ������
			{
				j--;
				continue;
			}

			short amount = GetCardInBox(i + 2);
			if (amount > 1)
				sCard_Box[i + 2][amount - 2].Visible = false; // ����� ��������
		}
	}

    Show();
}

bool Game::MoveCard(short s, short d) {
	short n;
	short s_end = 0;
	short d_end = 0;

	// ���� ������ ��������� ����� � ������
	for (n = 0; n < 53; n++) {
		s_end = n;
		if (sCard_Box[s][n].Value < 0)
			break;
	}
	for (n = 0; n < 53; n++) {
		d_end = n;
		if (sCard_Box[d][n].Value < 0)
			break;
	}

	if (s_end == 0)
		return (false); // ��������� ���� ����
	// ����� ��������� �����
	sCard_Box[d][d_end] = sCard_Box[s][s_end - 1];
	sCard_Box[s][s_end - 1].Value = -1; // ����� ��� ������ ���
	return (true);
}

void Game::RotatePool() {
	bool r = MoveCard(0, 1); // ���������� ����� �� �������� ����� � ������
	if (r == false) // ���� ���
	{
		// ���������� �������
		while (MoveCard(1, 0) == true);
	}
}

short Game::GetCardInBox(short box) {
	short amount = 0;

	for (short n = 0; n < 53; n++) {
		if (sCard_Box[box][n].Value < 0)
			break;
		amount++;
	}
	return (amount);
}

void Game::ChangeBox(short s_box, short s_index, short d_box) {
	short n;
	short d_end = 0;
	// ���� ������ ��������� ����� � ����� ����������
	for (n = 0; n < 52; n++) {
		d_end = n;
		if (sCard_Box[d_box][n].Value < 0)
			break;
	}
	// ���������� ���� ����� �� ���������� �����
	for (n = s_index; n < 52; n++, d_end++) {
		if (sCard_Box[s_box][n].Value < 0)
			break;
		sCard_Box[d_box][d_end] = sCard_Box[s_box][n];
		sCard_Box[s_box][n].Value = -1; // ����� ��� ������ ���
	}
}

void Game::ChangeCard(short s_box, short s_index, short d_box, short d_index) {
	if (d_box >= 2 && d_box < 9) // ���� ���� �� ������� ����
	{
		// ���� �� ����, �� ������ ���� ����� ������ ������
		if (d_index < 0) {
			if (sCard_Box[s_box][s_index].Value == 12) {
				ChangeBox(s_box, s_index, d_box);
				// ���� ����� - ������, ���������� �
				if (!sCard_Box[s_box][s_index - 1].Visible)
					sCard_Box[s_box][s_index - 1].Visible = true;
			}
			return;
		}
		// �����, ������ ����� � ������� �������� � ������ �������� ������
		if (sCard_Box[d_box][d_index].Value <= sCard_Box[s_box][s_index].Value)
			return; // �������� ����� ������, ��� ��, ��� ���� � ������ �����
		if (sCard_Box[d_box][d_index].Value >
			sCard_Box[s_box][s_index].Value + 1)
			return; // ����� ������ ������ �����, ������������ �� �������� �� 1
		CARD_SUIT md = sCard_Box[d_box][d_index].Suit;
		CARD_SUIT ms = sCard_Box[s_box][s_index].Suit;
		if ((md == SPADES || md == CLUBS) && (ms == SPADES || ms == CLUBS))
			return; // ����� ����� ���������
		if ((md == HEARTS || md == DIAMONDS) && (ms == HEARTS ||
			ms == DIAMONDS))
			return; // ����� ����� ���������

		ChangeBox(s_box, s_index, d_box); // �������� �����

		if (!sCard_Box[s_box][s_index - 1].Visible)
			sCard_Box[s_box][s_index - 1].Visible = true;
		return;
	}
	if (d_box >= 9 && d_box < 13) // ���� ���� �� ���� ������
	{
		// ���� ������� ��������� ����, �� ��� ���������� ����� ������ - ������ �� �����
		if (GetCardInBox(s_box) > s_index + 1)
			return;
		// ���� ���� ����, �� ������ ���� ����� ������ ����
		if (d_index < 0) {
			if (sCard_Box[s_box][s_index].Value == 0)
				// ���� ����� - ���, ���������� �
			{
				if (!sCard_Box[s_box][s_index - 1].Visible)
					sCard_Box[s_box][s_index - 1].Visible = true;
				ChangeBox(s_box, s_index, d_box);
			}
			return;
		}
		// �����, ������ ����� � ������� ����������� � ���������� �������� ������
		if (sCard_Box[d_box][d_index].Value > sCard_Box[s_box][s_index].Value)
			return; // �������� ����� ������, ��� ��, ��� ���� � ������ �����
		if (sCard_Box[d_box][d_index].Value + 1 <
			sCard_Box[s_box][s_index].Value)
			return; // ����� ������ ������ �����, ������������ �� �������� �� 1
		CARD_SUIT md = sCard_Box[d_box][d_index].Suit;
		CARD_SUIT ms = sCard_Box[s_box][s_index].Suit;
		if (ms != md)
			return; // ����� �� ���������

		ChangeBox(s_box, s_index, d_box);

		if (!sCard_Box[s_box][s_index - 1].Visible)
			sCard_Box[s_box][s_index - 1].Visible = true;
		return;
	}
}

void Game::Show() {
	for (short i = 0; i < 6; i++)
		for (short j = 0; j < 4; j++)
			device->Draw(i*Desk_Size, j*Desk_Size, desktop->Graphic);

	// Card_W*c + Card_sm*c , ��� �������� �� � ��� ������� ���������� �����!

	// ��������� ������ 0-1
	for (short i = 0; i < 2; i++) {
		device->Pen->Color = clBlack;

		short x = BOX_0_1->x + Card_W * i + Card_sm * i;
		short y = BOX_0_1->y;
		short colvo = GetCardInBox(i) - 1;

		if (colvo + 1 == 0) {
			device->Rectangle(x + 1, y + 1, x + Card_W, y + Card_H);
			continue;
		}
		device->Draw(x, y, sCard_Box[i][colvo].Pict->Graphic);

		// ��������� ��������--------------------
		if (sCard_Box[i][colvo].Select){
			device->Pen->Color = clLime;

			device->MoveTo(x, y);
			device->LineTo(x, y + Card_H);
			device->MoveTo(x + Card_W, y + 1);
			device->LineTo(x + Card_W, y + Card_H + 1);
		}
		// --------------------------------------
	}

	// ��������� ������ 2-8
	for (short c = 0, lol = 0, i = 2; i < 9; i++, c++, lol = 0) {
		for (short j = 0; j < 53; j++, lol++) {
			device->Pen->Color = clBlack;

			short x = BOX_2_8->x + Card_W * c + Card_sm * c;
			short y = BOX_2_8->y + Card_sm * 2 * lol;

			if (GetCardInBox(i) == 0)
				device->Rectangle(x + 1, y + 1, x + Card_W, y + Card_H);
			if (sCard_Box[i][j].Value < 0)
				break;

			if (sCard_Box[i][j].Visible)
				device->Draw(x, y, sCard_Box[i][j].Pict->Graphic);
			else
				device->Draw(x, y, back->Graphic);

			// ��������� ��������--------------------
			if (sCard_Box[i][j].Select){
				device->Pen->Color = clLime;

				device->MoveTo(x, y);
				device->LineTo(x, y + Card_H);
				device->MoveTo(x + Card_W, y + 1);
				device->LineTo(x + Card_W, y + Card_H + 1);
			}
			// --------------------------------------
		}
	}

	// ��������� ������ 9-12
	for (short c = 0, i = 9; i < 13; i++, c++) {
		device->Pen->Color = clBlack;

		short x = BOX_9_12->x + Card_W * c + Card_sm * c;
		short y = BOX_9_12->y;
		short colvo = GetCardInBox(i) - 1;

		if (colvo + 1 == 0) {
			device->Rectangle(x, y, x + Card_W, y + Card_H);
			continue;
		}
		device->Draw(x, y, sCard_Box[i][colvo].Pict->Graphic);

		// ��������� ��������--------------------
		if (sCard_Box[i][colvo].Select){
			device->Pen->Color = clLime;

			device->MoveTo(x, y);
			device->LineTo(x, y + Card_H);
			device->MoveTo(x + Card_W, y + 1);
			device->LineTo(x + Card_W, y + Card_H + 1);
		}
		// --------------------------------------
	}
}

void Game::Click(short x, short y) {
	TPoint *click = new TPoint(x, y);

	// ���� � 0
	if ((click->x > BOX_0_1->x && click->x < BOX_0_1->x + Card_W) && (click->y >
		BOX_0_1->y && click->y < BOX_0_1->y + Card_H)) {
		sCard_Box[Sel.col_num][Sel.card_num].Select = false;
		Sel.col_num = -1;
		RotatePool();
	}
	// ------------------------------------------------------------

	// ���� � 1
	if ((click->x > BOX_0_1->x + Card_W + Card_sm && click->x < BOX_0_1->x +
		Card_W * 2 + Card_sm) && (click->y > BOX_0_1->y && click->y <
		BOX_0_1->y + Card_H)) {
		short colvo = GetCardInBox(1) - 1;
		if (Sel.col_num == -1 && colvo + 1 > 0) {
			Sel.col_num = 1;
			Sel.card_num = colvo;
			sCard_Box[1][colvo].Select = true;
		}
		else {
			sCard_Box[Sel.col_num][Sel.card_num].Select = false;
			Sel.col_num = -1;
		}
	}
	// ------------------------------------------------------------

	// ���� � 2 - 8
	CkilBox2_8(x, y);
	// ------------------------------------------------------------

	// ���� � 9 - 12
	CkilBox9_12(x, y);
	// ------------------------------------------------------------

	Show();
}

void Game::CkilBox2_8(short x, short y) {
	TPoint *click = new TPoint(x, y);

	for (short lol = 0, c = 0, i = 2; i < 9; i++, c++, lol = 0) {
		short colvo = GetCardInBox(i) - 1;
		for (short j = 0; j < 53; j++, lol++) {
			if (colvo + 1 > 0) { // ���� ���� ������ 0
				if (sCard_Box[i][j].Value < 0)
					break;

				short x = BOX_2_8->x + Card_W * c + Card_sm * c;
				short y = BOX_2_8->y + Card_sm * 2 * lol;

				if (j == colvo) { // ���� ��������� �����
					if ((click->x > x && click->x < x + Card_W) && (click->y >
						y && click->y < y + Card_H)) { // Ÿ ����������
						if (Sel.col_num == -1) {
							Sel.col_num = i;
							Sel.card_num = j;
							sCard_Box[i][j].Select = true;
						}
						else {
							sCard_Box[Sel.col_num][Sel.card_num].Select = false;
							ChangeCard(Sel.col_num, Sel.card_num, i, j);
							Sel.col_num = -1;
						}
						return;
					}
				}
				else { // ���� �� ��������� �����
					if ((click->x > x && click->x < x + Card_W) && (click->y >
						y && click->y < y + Card_sm * 2) &&
						(sCard_Box[i][j].Visible == true)) { // Ÿ ����������
						if (Sel.col_num == -1) {
							Sel.col_num = i;
							Sel.card_num = j;
							sCard_Box[i][j].Select = true;
						}
						else {
							sCard_Box[Sel.col_num][Sel.card_num].Select = false;
							ChangeCard(Sel.col_num, Sel.card_num, i, j);
							Sel.col_num = -1;
						}
						return;
					}
				}
			}
			else { // ���� ���� ������ ����
				short x = BOX_2_8->x + Card_W * c + Card_sm * c;
				short y = BOX_2_8->y;

				if ((click->x > x && click->x < x + Card_W) && (click->y >
					y && click->y < y + Card_H)) { // Ÿ ����������
					if (Sel.col_num != -1) {
						sCard_Box[Sel.col_num][Sel.card_num].Select = false;
						ChangeCard(Sel.col_num, Sel.card_num, i, j - 1);
						Sel.col_num = -1;
					}
					return;
				}
				break;
			}
		}
	}
}

void Game::CkilBox9_12(short x, short y) {
	TPoint *click = new TPoint(x, y);

	for (short c = 0, i = 9; i < 13; i++, c++) {
		short colvo = GetCardInBox(i) - 1;
		short x = BOX_9_12->x + Card_W * c + Card_sm * c;
		short y = BOX_9_12->y;

		if ((click->x > x && click->x < x + Card_W) && (click->y >
			y && click->y < y + Card_H)) { // Ÿ ����������
			if (Sel.col_num == -1 && colvo + 1 > 0) {
				Sel.col_num = i;
				Sel.card_num = colvo;
				sCard_Box[i][colvo].Select = true;
			}
			else if (Sel.col_num != i) {
				sCard_Box[Sel.col_num][Sel.card_num].Select = false;
				ChangeCard(Sel.col_num, Sel.card_num, i, colvo);
				Sel.col_num = -1;
			}
			return;
		}
	}
}

bool Game::Win() {
	for (short i = 0; i < 9; i++)
		if (GetCardInBox(i) != 0)
			return false;

	return true;
}
