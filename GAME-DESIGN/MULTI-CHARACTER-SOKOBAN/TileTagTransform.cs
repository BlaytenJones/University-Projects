using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileTagTransform : MonoBehaviour
{
    public bool crate = false;
    // Start is called before the first frame update
    void Start()
    {
        foreach(Transform child in gameObject.transform) {
            if(child.CompareTag(gameObject.tag) || child.CompareTag("Walkable")) {
                child.gameObject.SetActive(true);
            } else {
                child.gameObject.SetActive(false || (child.gameObject.CompareTag("Crate") && crate));
            }
        }
        GameManager.Instance.numInitialized++;
    }

    // Update is called once per frame
    void Update()
    {
        if (gameObject.CompareTag("Laser")) {
            #pragma warning disable CS0618 // Type or member is obsolete
            gameObject.transform.FindChild("Laser").gameObject.SetActive(!GameManager.Instance.buttCondition);
            #pragma warning restore CS0618 // Type or member is obsolete
        }
    }
}
