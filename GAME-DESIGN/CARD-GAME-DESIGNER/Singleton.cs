using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class Singleton : MonoBehaviour
{
    public Text Name;
    public TextMeshProUGUI Mana;
    public TextMeshProUGUI HP;
    public TextMeshProUGUI ATK;
    public TextMeshProUGUI DEF;
    public TextMeshProUGUI INT;
    public TextMeshProUGUI Desc;
    public Text Effect;
    public TextMeshProUGUI CardNo;
    public Text Species;
    public TextMeshProUGUI Rarity;
    public TextMeshProUGUI Rarity2;
    public TextMeshProUGUI Variant;

    public GameObject Power;
    public GameObject Picture;
    public GameObject cardBaseCanvas;
    public GameObject elementCanvas;
    public GameObject seriesCanvas;
    public GameObject cardLining;
    public GameObject albumCanvas;

    public bool uploading = false;

    private bool switchColor;
    private int currSeries = 12;
    private int currElement = 0;
    private int currNodes = 0;

    public Vector2 originalSize;
    //Typing, Power, Nodes, Alignment, Series are visual changes

    private string Focus;

    public static Singleton instance;

    public Dictionary<string, string> details = new Dictionary<string, string>()
    {
        {"Name", "Name"},
        {"Mana", "0"},
        {"HP", "0"},
        {"ATK", "0"},
        {"DEF", "0"},
        {"INT", "0"},
        {"Desc", "Description"},
        {"Effect", "Effect"},
        {"CardNo", "1"},
        {"Element", "Matter"},
        {"Species", "Other"},
        {"Power", "1"},
        {"Nodes", "0"},
        {"Rarity", "C"},
        {"Alignment", "Neutral"},
        {"Series", "Other"},
        {"Variant", "0"},
        {"Album", ""},
        {"Type", "Character"}
    };

    void Awake()
    {
        if (instance == null)
        {
            instance = this;
        }
        Name.text = details["Name"]; Mana.text = details["Mana"]; HP.text = details["HP"]; ATK.text = details["ATK"]; DEF.text = details["DEF"]; INT.text = details["INT"]; Desc.text = details["Desc"];
        Effect.text = details["Effect"]; CardNo.text = "#" + int.Parse(details["CardNo"]).ToString("d3"); Species.text = details["Species"]; Rarity.text = details["Rarity"]; Rarity2.text = Rarity.text;
        Variant.text = (details["Variant"] == "0" || details["Variant"] == "") ? "" : "V" + details["Variant"];
    }

    public void changeVar(string detail)
    {
        details[Focus] = detail;
        Name.text = details["Name"]; Mana.text = details["Mana"]; HP.text = details["HP"]; ATK.text = details["ATK"]; DEF.text = details["DEF"]; INT.text = details["INT"];
        Desc.text = details["Desc"]; Effect.text = details["Effect"]; Species.text = details["Species"]; Rarity.text = details["Rarity"]; Rarity2.text = Rarity.text;
        Variant.text = (details["Variant"] == "0" || details["Variant"] == "") ? "" : "V" + details["Variant"];

        if(Focus == "CardNo")
        {
            cardNoShenanigans(detail);
        }

        if(Focus == "Rarity")
        {
            if (details["Rarity"] != "S")
            {
                Rarity.fontStyle = FontStyles.Bold;
            }
            else
            {
                Rarity.fontStyle = FontStyles.Normal;
            }
            cardNoShenanigans(CardNo.text);

            if (details["Rarity"] == "L")
            {
                cardLining.transform.GetChild(0).gameObject.SetActive(true);
                cardLining.transform.GetChild(1).gameObject.SetActive(false);
                Rarity.color = new Color(1.0f, 0.8431372549f, 0.3725490196f);
                Rarity.transform.localScale = new Vector3(0.07f, 0.07f, 1.0f);
                Rarity.rectTransform.localPosition = new Vector3(-424.64f, -238.18f, -9.65f);
                Rarity.enableVertexGradient = false;

                Rarity2.gameObject.SetActive(true);
                Rarity2.transform.localPosition = new Vector3(-424.79f, -238.21f, -9.65f);
                Rarity2.transform.localScale = new Vector3(0.07f, 0.07f, 1.0f);
            }
            else if (details["Rarity"] == "UL+")
            {
                cardLining.transform.GetChild(1).gameObject.SetActive(true);
                cardLining.transform.GetChild(0).gameObject.SetActive(false);
                Rarity.transform.localScale = new Vector3(0.06f, 0.06f, 1.0f);
                Rarity.rectTransform.localPosition = new Vector3(-424.64f, -237.7f, -9.65f);
                Rarity.color = Color.white;
                Rarity.enableVertexGradient = true;

                Rarity2.gameObject.SetActive(true);
                Rarity2.transform.localPosition = new Vector3(-424.76f, -237.72f, -9.65f);
                Rarity2.transform.localScale = new Vector3(0.06f, 0.06f, 1.0f);
            }
            else
            {
                cardLining.transform.GetChild(0).gameObject.SetActive(false);
                cardLining.transform.GetChild(1).gameObject.SetActive(false);
                Rarity.color = Color.black;
                Rarity.transform.localScale = new Vector3(0.07f, 0.07f, 1.0f);
                Rarity.rectTransform.localPosition = new Vector3(-424.75f, -238.04f, -9.65f);
                Rarity.enableVertexGradient = false;

                Rarity2.gameObject.SetActive(false);
            }
        }

        if(Focus == "Element")
        {
            bool totalCheck = false;
            for(int i = 0; i < cardBaseCanvas.transform.GetChild(currNodes).childCount; i++)
            {
                bool newCheck = cardBaseCanvas.transform.GetChild(currNodes).GetChild(i).gameObject.name.ToLower() == detail.ToLower();
                cardBaseCanvas.transform.GetChild(currNodes).GetChild(i).gameObject.SetActive(newCheck);
                elementCanvas.transform.GetChild(i).gameObject.SetActive(newCheck);
                currElement = (newCheck) ? i : currElement;
                totalCheck |= newCheck;
            }
            if (!totalCheck)
            {
                cardBaseCanvas.transform.GetChild(currNodes).GetChild(0).gameObject.SetActive(true);
                elementCanvas.transform.GetChild(0).gameObject.SetActive(true);
                currElement = 0;
            }
            switchColor = true;
        }

        if (Focus == "Nodes")
        {

            int num = 0;
            int.TryParse(detail, out num);
            if(num < 0)
            {
                num = 0;
            }
            else if(num > 3)
            {
                num = 3;
            }
            bool totalCheck = false;
            for (int i = 0; i < cardBaseCanvas.transform.childCount; i++)
            {
                bool newCheck = cardBaseCanvas.transform.GetChild(i).gameObject.name == num.ToString();
                cardBaseCanvas.transform.GetChild(i).gameObject.SetActive(newCheck);
                currNodes = (newCheck) ? i : currNodes;
                totalCheck |= newCheck;
                if (newCheck)
                {
                    for (int j = 0; j < cardBaseCanvas.transform.GetChild(currNodes).childCount; j++)
                    {
                        cardBaseCanvas.transform.GetChild(i).GetChild(j).gameObject.SetActive(false);
                        elementCanvas.transform.GetChild(i).gameObject.SetActive(false);
                    }
                    cardBaseCanvas.transform.GetChild(i).GetChild(currElement).gameObject.SetActive(true);
                    elementCanvas.transform.GetChild(currElement).gameObject.SetActive(true);
                }
            }
            if (!totalCheck)
            {
                cardBaseCanvas.transform.GetChild(0).gameObject.SetActive(true);
                currNodes = 0;
            }
        }

        if (Focus == "Series")
        {
            //000000 is black
            bool totalCheck = false;
            for (int i = 0; i < seriesCanvas.transform.childCount; i++)
            {
                bool newCheck = seriesCanvas.transform.GetChild(i).gameObject.name.ToLower() == detail.ToLower();
                seriesCanvas.transform.GetChild(i).gameObject.SetActive(newCheck);
                currSeries = (newCheck) ? i : currSeries;
                totalCheck |= newCheck;
            }
            if (!totalCheck)
            {
                seriesCanvas.transform.GetChild(seriesCanvas.transform.childCount - 1).gameObject.SetActive(true);
                currSeries = cardBaseCanvas.transform.childCount - 1;
            }
        }

        if(Focus == "Alignment")
        {
            switchColor = true;
        }

        if(Focus == "Album")
        {
            for (int i = 0; i < albumCanvas.transform.childCount; i++)
            {
                albumCanvas.transform.GetChild(i).gameObject.SetActive((albumCanvas.transform.GetChild(i).gameObject.name.ToLower() == detail.ToLower()));
            }
        }
    }

    public void changePower(string detail)
    {
        int num = 0;
        int.TryParse(detail, out num);
        if (num < 1)
        {
            num = 1;
            detail = "1";
        }else if(num > 5)
        {
            num = 5;
            detail = "5";
        }
        details["Power"] = detail;
        for (int i = 0; i < 5; i++)
        {
            Power.transform.GetChild(i).gameObject.SetActive(i + 1 == num);
        }
    }

    public void changeNodes(string detail)
    {
        int num = 0;
        int.TryParse(detail, out num);
        if (num < 0)
        {
            num = 0;
            detail = "0";
        }
        else if (num > 3)
        {
            num = 3;
            detail = "3";
        }
        details["Nodes"] = detail;
    }

    public void currFocus(string focus)
    {
        Focus = focus;
    }

    void Update()
    {
        if (switchColor)
        {
            switch (details["Alignment"].ToLower())
            {
                case "good":
                    seriesCanvas.transform.GetChild(currSeries).gameObject.GetComponent<SpriteRenderer>().color = new Color(0.32549019607f, 0.75294117647f, 1.0f, 1.0f); ;
                    break;
                case "evil":
                    seriesCanvas.transform.GetChild(currSeries).gameObject.GetComponent<SpriteRenderer>().color = new Color(0.50980392156f, 0.18039215686f, 0.19215686274f, 1.0f);
                    break;
                //neutral
                default:
                    string tempElement = elementCanvas.transform.GetChild(currElement).gameObject.name;
                    if (tempElement == "Matter" || tempElement == "Chaos" || tempElement == "Mind")
                    {
                        seriesCanvas.transform.GetChild(currSeries).gameObject.GetComponent<SpriteRenderer>().color = Color.white;
                    }
                    else
                    {
                        seriesCanvas.transform.GetChild(currSeries).gameObject.GetComponent<SpriteRenderer>().color = Color.black;
                    }
                    break;
            }
            switchColor = false;
        }
    }

    public void setScale(string scale)
    {
        float num = 0.0f;
        float.TryParse(scale, out num);
        if(num > 0)
        {
            Picture.transform.GetComponent<RectTransform>().sizeDelta = num * originalSize;
            Picture.GetComponent<BoxCollider2D>().size = num * originalSize;
        }
    }

    public void cardNoShenanigans(string detail)
    {
        //I wrote this code after 3-4 hours of coding at 12:00 AM. Essentially, it is an extremely inefficient way to update the card number in case the user has already put in a number for the card number but goes back to change
        //the rarity to special. It will attempt to detect how many numbers the previous card number was by subtracting the # and SP (if it exists) and taking the rest of the characters and compiling them into a new string to use in
        //the new string. This ensures it works both ways!
        int num = 0;
        if (detail != "")
        {
            if (detail[0] != '#')
            {
                int.TryParse(detail, out num);
            }
            else
            {
                string tempString = detail.Substring(1 + ((detail[1] == 'S') ? 2 : 0));
                int.TryParse(tempString, out num);
            }
            CardNo.text = "#" + ((details["Rarity"] != "S") ? "" : "SP") + num.ToString("d3");
            if (CardNo.text.Length > 4)
            {
                CardNo.transform.localScale = new Vector3(.07f / (Mathf.Pow(1.25f, (CardNo.text.Length - 4))), .07f, 1.0f);
            }
            else
            {
                CardNo.transform.localScale = new Vector3(.07f, .07f, 1.0f);
            }
        }
    }

    public void changeCardType(string Type)
    {
        details["Type"] = Type;

    }
}