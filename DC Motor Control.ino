const int pwm = 9;
const int dir = 10;

void setup() {
	pinMode(dir,OUTPUT);
}
void loop() {
	DC (255, HIGH);
}
void DC (unsigned char p, unsigned char d)
{
digitalWrite(dir,d);
if(d== LOW)
	{
	analogWrite(pwm,p);
	}
else
	{
	analogWrite(pwm,255 - p);
	}
}