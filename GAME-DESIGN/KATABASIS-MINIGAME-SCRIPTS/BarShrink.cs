using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BarShrink : MonoBehaviour
{
    //if false, it is boost, otherwise it is HP. This cuts down on code
    public bool HP = false;

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        //Decides if it is editing based on HP or boost and then shrinks and moves it based on current ratio.
        transform.localScale = new Vector3(1.1f * (HP ? (Gamemanager3.instance.HP / 30.0f) : (Gamemanager3.instance.boostVal / 30.0f)), 1, .28f);
        transform.localPosition = new Vector3((1.1f - transform.localScale.x) * (HP ? -1 : 1) * 5, 0, -5);
    }
}
