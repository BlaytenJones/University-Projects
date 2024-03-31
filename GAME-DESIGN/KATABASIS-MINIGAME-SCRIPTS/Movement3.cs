using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement3 : MonoBehaviour
{
    Rigidbody player;
    public float speed = 10f;
    public float pullForce = 4.2f;
    private float currPull = 0f;
    private float zValue;
    // Start is called before the first frame update
    void Start()
    {
        player = GetComponent<Rigidbody>();
        zValue = transform.position.z;
    }

    // Update is called once per frame
    void Update()
    {
        if(currPull < pullForce)
        {
            currPull += .005f;
        }

        if (Input.GetKeyDown("space"))
        {
            if(Gamemanager3.instance.boostVal > 0)
            {
                Gamemanager3.instance.boost = true;
            }
        }
        if (Input.GetKeyUp("space"))
        {
            Gamemanager3.instance.boost = false;
        }

        //Store user input as a movement vector
        Vector3 input = new Vector3(Input.GetAxis("Horizontal"), Input.GetAxis("Vertical"), 0);
        Vector3 pullDir = (new Vector3(0, 1, zValue) - transform.position);
        if (!Gamemanager3.instance.pause)
        {
            player.AddForce(((input * speed) + (pullDir * currPull * (Gamemanager3.instance.boost ? .1f : 1))) * Time.deltaTime, ForceMode.Impulse);
        }
    }

    void OnTriggerStay(Collider other)
    {
        if(other.tag == "Enemy")
        {
            Gamemanager3.instance.HP -= .08f;
        }
    }
}
