
struct VSinput
{
    float4 position : POSITION;
};

struct VSoutput
{
    float4 position : POSITION;
};

float4 main( VSinput input)
{
    VSoutput output;
    output.position = input.position;
	return output;
}