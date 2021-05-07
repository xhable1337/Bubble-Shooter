#define MODELS 2 // ����� ������� ��� ���� (1 - �������, 2 - ������)

//// ������ ��� ����
char* menuBG = "./Images/menuBG.png";                           // ��� ��� ����			            
char* menuBG2 = "./Images/BG.png";                              // ��� � ������� ����
char* arrow = "./Images/arrow.png";								// ������� ��� ����
char* topPlayers = "./Images/topPlayers.png";                   // ������� ��������
char* credits = "./Images/credits.png";                         // ����� ����������


//// ������������� ������ ����������	
// ���
char* gameBG = "./Images/BG2.png";                              // ��� �� ����� ����

// ������ ������� ������							            
char* uiBlue = "./Images/uiBlue.png";				            // ����� ������� UI
char* menuBlue = "./Images/menuBlue.png";			            // ����� ������ �������� ����
char* menuHoverBlue = "./Images/menuHoverBlue.png";             // ����� ������ �������� ���� ��� ��������� ����
char* soundOnBlue = "./Images/soundOnBlue.png";		            // ����� ������ "���� �������"
char* soundOnHoverBlue = "./Images/soundOnHoverBlue.png";       // ����� ������ "���� �������" ��� ��������� ����
char* soundOffBlue = "./Images/soundOffBlue.png";	            // ����� ������ "���� ��������"
char* soundOffHoverBlue = "./Images/soundOffHoverBlue.png";     // ����� ������ "���� �������" ��� ��������� ����
char* lifeBlue = "./Images/lifeBlue.png";                       // ����� ��������� ������
                                                                
// ������� ������� ������							            
char* uiYellow = "./Images/uiYellow.png";                       // Ƹ���� ������� UI
char* menuYellow = "./Images/menuYellow.png";                   // Ƹ���� ������ �������� ����
char* menuHoverYellow = "./Images/menuHoverYellow.png";         // Ƹ���� ������ �������� ���� ��� ��������� ����
char* soundOnYellow = "./Images/soundOnYellow.png";             // Ƹ���� ������ "���� �������"
char* soundOnHoverYellow = "./Images/soundOnHoverYellow.png";   // Ƹ���� ������ "���� �������" ��� ��������� ����
char* soundOffYellow = "./Images/soundOffYellow.png";           // Ƹ���� ������ "���� ��������"
char* soundOffHoverYellow = "./Images/soundOffHoverYellow.png"; // Ƹ���� ������ "���� ��������" ��� ��������� ����
char* lifeYellow = "./Images/lifeYellow.png";                   // Ƹ���� ��������� ������
                                                        
// ������� ������� ������							    
char* uiRed = "./Images/uiRed.png";							    // ������� ������� UI
char* menuRed = "./Images/menuRed.png";						    // ������� ������ �������� ����
char* menuHoverRed = "./Images/menuHoverRed.png";			    // ������� ������ �������� ���� ��� ��������� ����
char* soundOnRed = "./Images/soundOnRed.png";				    // ������� ������ "���� �������"
char* soundOnHoverRed = "./Images/soundOnHoverRed.png";         // ������� ������ "���� �������" ��� ��������� ����
char* soundOffRed = "./Images/soundOffRed.png";				    // ������� ������ "���� ��������"
char* soundOffHoverRed = "./Images/soundOffHoverRed.png";       // ������� ������ "���� ��������" ��� ��������� ����
char* lifeRed = "./Images/lifeRed.png";						    // ������� ��������� ������

// ��������
char* defeat = "./Images/defeat.png";                           // �����-����� ��� ���������
char* returnRed = "./Images/returnRed.png";                     // ������� ������ ����������� 
char* returnHoverRed = "./Images/returnHoverRed.png";           // ������� ������ ����������� ��� ���������
char* rankRed = "./Images/rankRed.png";                         // ������� ������ ������� ��������
char* rankHoverRed = "./Images/rankHoverRed.png";               // ������� ������ ������� �������� ��� ���������

// �������
char* victory = "./Images/victory.png";                         // �����-����� ��� ������
char* returnBlue = "./Images/returnBlue.png";                   // ����� ������ ����������� 
char* returnHoverBlue = "./Images/returnHoverBlue.png";		    // ����� ������ ����������� ��� ���������
char* rankBlue = "./Images/rankBlue.png";					    // ����� ������ ������� ��������
char* rankHoverBlue = "./Images/rankHoverBlue.png";			    // ����� ������ ������� �������� ��� ���������

//// ������������� ������
// ������
#if (MODELS == 1)
char* color1 = "./Images/Earth.png";
char* color2 = "./Images/Saturn.png";
char* color3 = "./Images/Neptune.png";
char* color4 = "./Images/Venus.png";
char* color5 = "./Images/Jupiter.png";
char* color6 = "./Images/Mars.png";
#elif (MODELS == 2)
char* color1 = "./Images/red.png";
char* color2 = "./Images/orange.png";
char* color3 = "./Images/yellow.png";
char* color4 = "./Images/green.png";
char* color5 = "./Images/blue.png";
char* color6 = "./Images/purple.png";
#endif